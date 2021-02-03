//#include "kernel.h"

enum object_type {
	SPHERE,
	CONE,
	TRIANGLE,
	CYLINDER,
	PLANE,
	ELLIPSOID,
	HYPERBOLOID,
	PARABOLOID,
	BOX,
	TORUS
};

enum light_type{
	POINT,
	AMBIENT,
	DIRECTIONAL
};

typedef struct		s_basis
{
	float3			u;
	float3			v;
	float3			w;
}					t_basis;

typedef	struct		s_light
{
	float3			position;
	float3			direction;
	float			intensity;
	int				type;
}					t_light;

typedef struct		s_color
{
	uchar			red;
	uchar			green;
	uchar			blue;
	uchar			alpha;
}					t_color;

typedef	struct		s_material
{
	t_color			color;
	float			specular;
	float			reflection;
}					t_material;

typedef struct		s_sphere
{
	float3			center;
	float			radius;
}					t_sphere;

typedef struct		s_plane
{
	float3			normal;
	float3			point;
	float			d;
}					t_plane;

typedef struct		s_cylinder
{
	float3			position;
	float3			vec;
	float			radius;
}					t_cylinder;

typedef struct		s_cone
{
	float3			position;
	float3			vec;
	float			angle;
}					t_cone;

typedef struct		s_triangle
{
	float3			vertex[3];
	float3			normal;
}					t_triangle;

typedef	struct		s_box
{
	float3			a;
	float3			b;
	float			face_hit;
}					t_box;

typedef	struct		s_ellipsoid
{
	float3			center1;
	float3			center2;
	float			radius;
}					t_ellipsoid;

typedef struct		s_paraboloid
{
	float3			center;
	float3			vec;
	float			k;
}					t_paraboloid;

typedef struct		s_torus
{
	float3			center;
	float3			vec;
	float			radius1;
	float			radius2;
}					t_torus;

typedef	union		primitive
{
	t_cylinder		cylinder;
	t_cone			cone;
	t_sphere		sphere;
	t_plane			plane;
	t_triangle		triangle;
	t_ellipsoid		ellipsoid;
	t_box			box;
	t_paraboloid	paraboloid;
	t_torus			torus;
}					t_primitive;

typedef	struct		 s_cutting_surface
{
	t_primitive		primitive;
	int				type;
	int				is_negative;
}					t_cutting_surface;

typedef struct		s_object3d_d
{
	t_primitive		primitive;
	t_basis			basis;
	float3			rotation;
	t_color			color;
	float			specular;
	float			roughness;
	float			refraction;
	float			reflection;
	int				color_disrupt;
	int				type;
	int				texture_id;
	int				texture_size;
	int				texture_width;
	int				texture_height;
	int				l_size;
	int				normal_map_id; //разметка частей текстурного буфера для поиска карты нормалей
	int				texture_size_nm;
	int				texture_width_nm;
	int				texture_height_nm;
	int				l_size_nm;
}					t_object_d;

float3  change_basis(float3 vec, t_basis basis)
{
    float3  tmp;
	tmp.x = dot(vec, basis.v);
	tmp.y = dot(vec, basis.u);
	tmp.z = dot(vec, basis.w);
    return (tmp);
}

float3  mapping_plane(float3 t, t_object_d obj)
{
    float3 p;
    t -= obj.primitive.plane.point;
    t = change_basis(t, obj.basis);
    t.x /= 10;
    t.y /= 10;
    t.z /= 10;
    p.x = t.x;
    p.y = t.z;
    p.z = 0;
    return (p);
}

float3		mapping_triangle(float3 t, t_object_d obj)
{
	float3 p;

	t -= obj.primitive.triangle.vertex[0];
	t = change_basis(t, obj.basis);
	t.x /= 1000;
	t.y /= 1000;
	t.z /= 1000;
	p.x = t.x;
	p.y = t.z;
	p.z = 0;
	return (p);
}

float3		mapping_cone(float3 t, t_object_d obj)
{
	float3 p;
	float3 tmp;

	t -= obj.primitive.cone.position;
	t = change_basis(t, obj.basis);
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) / 1.5707963267948;
	phi = tmp.y > 0 ? 1.f - phi : phi;
	t.x /= 20;
	t.y /= 20;
	t.z /= 20;
	p.x = fabs(-phi);
	p.y = fabs(t.y);
	p.z = 0;
	return (p);
}

float3		mapping_cylinder(float3 t, t_object_d obj)
{
	float3 p;

	t -= obj.primitive.cylinder.position;
	t = change_basis(t, obj.basis);
	float phi = acos(t.x / obj.primitive.cylinder.radius) / 1.5707963267948;
	phi = t.z > 0 ? 1.f - phi : phi;
	t /= obj.primitive.cylinder.radius;
	p.x = phi;
	p.y = -t.y;
	p.z = 0;
	return (p);
}

float3		mapping_sphere(float3 t, t_object_d obj)
{
 	float3 p;
	float3 tmp;

	t -= obj.primitive.sphere.center;
	t = change_basis(t, obj.basis);
	t /= obj.primitive.sphere.radius;
	float theta = acos(t.y) / 3.14159265358979;
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) / 1.5707963267948;
	phi = t.z > 0 ? 1.f - phi : phi;
	p.x = phi;
	p.y = theta;
	p.z = 0;
    return(p);
}

float3 text_map_select(t_object_d obj, float3 t)
{
    float3 p;

    if (obj.type == SPHERE)
		p = mapping_sphere(t, obj);
 	if (obj.type == PLANE)
		p = mapping_plane(t, obj);
	if (obj.type == CYLINDER)
		p = mapping_cylinder(t, obj);
	if (obj.type == CONE)
		p = mapping_cone(t, obj);
	if (obj.type == TRIANGLE)
		p = mapping_triangle(t, obj);
    return (p);
}

t_color	get_color_tex(__global uchar  *texture, float x, float y, t_object_d obj, int key)
{
 	if (x < 0)
		x = obj.texture_width + x;
	if (y < 0)
		y = obj.texture_height + y;
	int fx = obj.texture_width - (int)(obj.texture_width * x) % obj.texture_width;
	int fy = (int)( obj.texture_height * y) % obj.texture_height;
 	t_color c;
	c.red = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 2];
	c.green = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 1];
	c.blue = texture[obj.texture_id + 4 * fx + obj.l_size * fy];
	c.alpha = texture[obj.texture_id + 4 * fx + obj.l_size * fy + 4];
    return(c);
}

void		set_color_zero(__private t_color *color)
{
	color->red = 0;
	color->green = 0;
	color->blue = 0;
	color->alpha = 0;
}

void		copy_color(__private t_color *dst, __private t_color *src)
{
	dst->red = src->red;
	dst->blue = src->blue;
	dst->green = src->green;
	dst->alpha = src->alpha;
}

__kernel void    get_material_buf_cl(__global uchar *texture_data,\
                                     __global t_object_d *obj,\
                                    __global int *index_buf, \
                                    __global float3 *intersection_buf,\
                                    __global t_material *material_buf, \
									__global int *original_index_buf, \
									__global int *prev_index_buf, \
									int bounce_cnt)
{
    int i = get_global_id(0);
	float3 t;
    if (index_buf[i] != -1)
	{
		if (obj[index_buf[i]].texture_id != -1)
		{
			t = text_map_select(obj[index_buf[i]], intersection_buf[i]);
			material_buf[i].color = get_color_tex(texture_data, t.x, t.y, obj[index_buf[i]], i);
		}
		else
			material_buf[i].color = obj[index_buf[i]].color;
		material_buf[i].specular = obj[index_buf[i]].specular;
		material_buf[i].reflection = obj[index_buf[i]].reflection;
    }
 	else
	{
		if (bounce_cnt == 0)
		{
			material_buf[i].color.red = 0;
			material_buf[i].color.green = 0;
			material_buf[i].color.blue = 0;
			material_buf[i].color.alpha = 0;
			material_buf[i].specular = -1;
			material_buf[i].reflection = 0.0;
		}
		material_buf[i].reflection = 0.0;
	}
}
