//#include "kernel.h"

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

typedef	struct		s_ellipsoid
{
	float			radius;
	float3			center1;
	float3			center2;
}					t_ellipsoid;

typedef	struct		s_box
{
	float3			a;
	float3			b;
}					t_box;

typedef struct		s_paraboloid
{
	float			k;
	float3			center;
}					t_paraboloid;

typedef struct		s_torus
{
	float			radius1;
	float			radius2;
	float3			center;
}					t_torus;

typedef	union		primitive
{
	t_cylinder		cylinder;
	t_cone			cone;
	t_sphere		sphere;
	t_plane			plane;
	t_triangle		triangle;
	t_ellipsoid		ellipsoid;
	t_paraboloid	paraboloid;
	t_box			box;
	t_torus			torus;
}					t_primitive;

typedef	struct		s_cutting_surface
{
	int 			type;
	t_sphere		sphere;
	t_plane			plane;
	t_triangle		triangle;
	t_cone			cone;
	t_cylinder		cylinder;
}					t_cutting_surface;

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
}					t_object_d;

float cone_intersection(t_cone cone, float3 ray_start, float3 ray_dir)
{
	float t1;
    float t2;
    float b;
    float c;

    float3 dist = ray_start - cone.position;
	float a = dot(ray_dir, cone.vec);
	a = dot(ray_dir, ray_dir) - (1 + cone.angle * cone.angle) * a * a;
    b = 2 * (dot(ray_dir, dist) - (1 + cone.angle * cone.angle) * \
		dot(ray_dir, cone.vec) * dot(dist, cone.vec));
    c = dot(dist, cone.vec);
	c = dot(dist, dist) - (1 + cone.angle * cone.angle) * c * c;
	c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
	}
	return (0);
}

float cylinder_intersection(t_cylinder cyl, float3 ray_start, float3 ray_dir)
{
	float t1;
    float t2;
    float b;
    float c;

    float3 dist = ray_start - cyl.position;
	float a = dot(ray_dir, cyl.vec);
	a = dot(ray_dir, ray_dir) - a * a;
    b = 2 * (dot(ray_dir, dist) - dot(ray_dir, cyl.vec) * \
		dot(dist, cyl.vec));
    c = dot(dist, cyl.vec);
	c = dot(dist, dist) - c * c - cyl.radius * cyl.radius;
    c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
	}
	return (0);
}

float sphere_intersection(t_sphere sphere, float3 ray_start, float3 ray_dir)
{
    float a = dot(ray_dir, ray_dir);
    float b;
    float c;
    float t1;
    float t2;
    float3 dist = ray_start - sphere.center;
    b = 2 * dot(dist, ray_dir);
    c = dot(dist, dist) - (sphere.radius * sphere.radius);
    c = b * b - 4 * a * c;
    if (c >= 0)
    {
        c = sqrt(c);
        t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
    }
    return (0);
}

float triangle_intersection(t_triangle triangle, float3 ray_start, float3 ray_dir)
{
	float3 edge1;
	float3 edge2;
    float3 vec1;
	float3 vec2;
	float3 vec3;
    float det;
    float uv1;
	float uv2;

    edge1 = triangle.vertex[1] - triangle.vertex[0];
	edge2 = triangle.vertex[2] - triangle.vertex[0];
	vec1 = cross(ray_dir, edge2);
	det = dot(edge1, vec1);
	if (det < 1e-8 && det > -1e-8)
		return (0);
	det = 1 / det;
	vec2 = ray_start - triangle.vertex[0];
	uv1 = dot(vec2, vec1) * det;
	if (uv1 < 0 || uv1 > 1)
		return (0);
	vec3 = cross(vec2, edge1);
	uv2 = dot(ray_dir, vec3) * det;
	if (uv2 < 0 || uv1 + uv2 > 1)
		return (0);
	float res;
	res = dot(edge2, vec3) * det;
	return (res);
}

float plane_intersection(t_plane plane, float3 ray_start, float3 ray_dir)
{
	float k1;
	float k2;

    if ((dot(ray_dir, plane.normal)) == 0)
		return (0);
	k1 = dot(ray_start, plane.normal) + plane.d;
	k2 = dot(ray_dir, plane.normal);
	if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
		return (0);
	k1 = -k1 / k2;
	return (k1);
}

int			in_shadow(int index, float3 l, __global float3 *intersection_buf, \
						int obj_nmb, __global t_object_d *obj)
{
	float3	ray_start;
	int		i;
	float	t;

	i = 0;
	ray_start = l * 0.001f;
	ray_start += intersection_buf[index];
	while (i < obj_nmb)
	{
		if (obj[i].type == SPHERE)
			t = sphere_intersection(obj[i].primitive.sphere, ray_start, l);
		if (obj[i].type == TRIANGLE)
			t = triangle_intersection(obj[i].primitive.triangle, ray_start, l);
		if (obj[i].type == PLANE)
			t = plane_intersection(obj[i].primitive.plane, ray_start, l);
		if (obj[i].type == CONE)
			t = cone_intersection(obj[i].primitive.cone, ray_start, l);
		if (obj[i].type == CYLINDER)
			t = cylinder_intersection(obj[i].primitive.cylinder, ray_start, l);
		if (t < 1.0f && t > 0.00001f)
			break ;
		i++;
	}
	if (t < 1.0f && t > 0.0001f)
		return (1);
	return (0);
}

float		get_specular(int index, int j, float3 l, \
						__global float3 *normal_buf, \
						__global t_light *light, \
 						__global t_material *material_buf, \
						__global float3 *ray_buf)
{
	float		n;
	float		r;
	float		i;
 	float3		rad;
	float3		d;
	float3		lb;

	lb = l / length(l);
	i = 0;
	n = dot(normal_buf[index], lb);
	rad = normal_buf[index] * 2.0f;
	rad = rad * n;
	rad = rad - lb;
	d.x = -ray_buf[index].x;
	d.y = -ray_buf[index].y;
	d.z = -ray_buf[index].z;
	r = dot(rad, d);
	if (r > 0)
		i += light[j].intensity * pow((float)r / \
		(length(rad) * length(d)), \
		material_buf[index].specular);
	return (i);
}

float3		get_light_vec(int index, int j, __global float3 *intersection_buf, __global t_light *light)
{
	float3 light_vec;

	light_vec = 0;
	if (light[j].type == POINT)
		light_vec = light[j].position - intersection_buf[index];
	if (light[j].type == DIRECTIONAL)
		light_vec = light[j].direction;
	return (light_vec);
}

t_color		reflection_color(__global t_color *frame_buf, \
							__global float3 *ray_buf, \
                            __global float3 *intersection_buf, \
                            __global float3 *normal_buf, \
                            __global int *index_buf, \
                            __global t_material *material_buf, \
                            __global t_object_d *obj, \
                            __global t_light *light, \
							int light_nmb, \
							int index, int obj_nmb, int bounce_cnt)
{
	float		i;
	float3		l;
	float		n_dot_l;
	int			j;

	j = -1;
	i = 0;
	while (++j < light_nmb)
	{
		if (light[j].type == AMBIENT)
			i += light[j].intensity;
		else
		{
			l = get_light_vec(index, j, intersection_buf, light);
			n_dot_l = dot(normal_buf[index], l);
			if (!(in_shadow(index, l, intersection_buf, obj_nmb, obj)) && n_dot_l > 0)
			{
				if (material_buf[index].specular != -1)
					i += get_specular(index, j, l, normal_buf, light, material_buf, ray_buf);
				i += (light[j].intensity * n_dot_l) / length(l);
			}
		}
	}
	i = i > 1 ? 1 : i;
	t_color result;
	if (material_buf[index].reflection > 0.001f || bounce_cnt == 0)
	{
		result.red = material_buf[index].color.red * i;
		result.green = material_buf[index].color.green * i;
		result.blue = material_buf[index].color.blue * i;
	}
	else
	{
		result.red = frame_buf[index].red;
		result.green = frame_buf[index].green;
		result.blue = frame_buf[index].blue;
	}
	
	if (index == 1280 * 360 + 640 * 11)
		printf("result color device in reflection_color (%hhu, %hhu, %hhu)\n", result.red, result.green, result.blue);
	return (result);
}

__kernel void get_frame_buf_cl(__global t_color *frame_buf, \
                            __global float3 *ray_buf, \
                            __global float3 *intersection_buf, \
                            __global float3 *normal_buf, \
                            __global int *index_buf, \
                            __global t_material *material_buf, \
                            __global t_object_d *obj, \
                            __global t_light *light, \
                            int light_nmb,\
							int obj_nmb, int bounce_cnt)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	if (j != -1)
	{
		frame_buf[i] = reflection_color(frame_buf, ray_buf, intersection_buf, \
										normal_buf, index_buf, material_buf, \
										obj, light, light_nmb, i, obj_nmb, bounce_cnt);
	}
	else
	{
		frame_buf[i].red = 0;
		frame_buf[i].green = 0;
		frame_buf[i].blue = 0;
		frame_buf[i].alpha = 255;
	}
}
