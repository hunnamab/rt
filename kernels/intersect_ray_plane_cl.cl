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
	float			refraction;
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
	float3			center;
	float			a;
	float			b;
	float			c;
}					t_ellipsoid;

typedef	struct		s_box
{
	float3			a;
	float3			b;
	float			face_hit;
}					t_box;

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

typedef struct		s_hyperboloid
{
	float3			center;
	float			a;
	float			b;
	float			c;
}					t_hyperboloid;

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

typedef	struct		 	s_cutting_surface
{
	float3			param1;
	float3			param2;
	int				type;
	int				object;
	int				is_negative;
	float			param3;
}						t_cutting_surface;

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
	int				t_scale;
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

int cut(float3 point, __global t_cutting_surface *cs, int cs_nmb)
{
    int i;
    float result;
    i = 0;
    while(i < cs_nmb)
    {
        if (cs[i].type == PLANE)
        {
            result = cs[i].param1.x * point.x + cs[i].param1.y * point.y + cs[i].param1.z * point.z + cs[i].param3;
            if (result >= 0)
                return (0);
        }
		if(cs[i].type == SPHERE)
		{
			float3 buf;
			buf = point - cs[i].param1;
			result = length(buf);
			if (result >= cs[i].param3 && !cs[i].is_negative)
				return(0);
			if(result <= cs[i].param3 && cs[i].is_negative)
				return(0);
		}
        i++;
    }
    return (1);
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

float3 refract(float3 I, float3 N)
{
	float3 Nrefr = N;
	float NdotI = dot(Nrefr, I);
	float etai = 1;
	float etat = 1.5;
	float swap;
	if (NdotI < 0)
	{
		NdotI = -NdotI;
	}
	else
	{
		Nrefr = -N;
		swap = etai;
		etai = etat;
		etat = swap;
	}
	float eta = etai / etat;
	float k = 1 - (eta * eta) * (1 - NdotI * NdotI);
	if (k < 0)
		return (0);
	else
	{
		return (eta * I + (eta * NdotI - sqrt(k)) * Nrefr);
	}
	return (0);
}

__kernel void intersect_ray_plane_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_plane plane, \
                                __global int *index_buf, \
                                int index, int bounce_cnt, \
								__global t_cutting_surface *cs, \
								int cs_nmb, __global t_material *material_buf, \
								int is_refractive, __global float3 *normal_buf, \
								int refraction)
{
    int i = get_global_id(0);
    
	float k1;
	float3 ray;
	if (is_refractive == 1 && material_buf[i].refraction > 0.0)
	{
		ray_arr[i] = refract(ray_arr[i], normal_buf[i]);
	}
	else if (is_refractive == 1 && material_buf[i].refraction == 0.0)
		return ;
    if (bounce_cnt > 0)
    	ray = camera_start[i] + ray_arr[i] * 0.00001f;
	else if (is_refractive == 1)
		ray = camera_start[i] * 0.00001f;
	else
		ray = camera_start[i];
	if (bounce_cnt == 0 || material_buf[i].reflection > 0.0)
		k1 = plane_intersection(plane, ray, ray_arr[i]);
	else
		return ;
    if (k1 < depth_buf[i] && k1 > 0.01)
    {
		float3 intersection_point;
        intersection_point = ray_arr[i] * k1;
        intersection_point = intersection_point + camera_start[i];
        if (cut(intersection_point, cs, cs_nmb))
        {
			depth_buf[i] = k1;
			index_buf[i] = index;
		}
    }
}
