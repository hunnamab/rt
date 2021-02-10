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
	t_hyperboloid	hyperboloid;
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
	int				is_local;
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

float3 refract(float3 I, float3 N, float refr)
{
	float3 Nrefr = N;
	float NdotI = dot(Nrefr, I);
	float etai = 1;
	float etat = refr;
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

__kernel void get_refraction_ray_cl(__global float3 *ray_arr, \
                                __global int *index_buf, \
                                __global float3 *normal_buf, \
								__global int *exception_buf, \
                                __global t_object_d *obj)
{
    int i = get_global_id(0);
	
	if (index_buf[i] != -1)
	{
		if (obj[index_buf[i]].refraction > 0.0)
		{
			normal_buf[i] = refract(ray_arr[i], normal_buf[i], obj[index_buf[i]].refraction);
			exception_buf[i] = index_buf[i];
		}
		else
			return ;
	}
	else
		return ;
}
