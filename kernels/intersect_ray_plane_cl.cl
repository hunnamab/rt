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
	float3			center1;
	float3			center2;
	float			radius;
}					t_ellipsoid;

typedef	struct		s_box
{
	float3			a;
	float3			b;
}					t_box;

typedef struct		s_paraboloid
{
	float3			center;
	float			k;
}					t_paraboloid;

typedef struct		s_torus
{
	float3			center;
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

__kernel void intersect_ray_plane_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_plane plane, \
                                __global int *index_buf, \
                                int index, float reflection, int bounce_cnt)
{
    int i = get_global_id(0);
    
	float k1;
	float3 ray;
    ray = camera_start[i] + ray_arr[i] + 0.001f;
	//if (reflection > 0.001f || bounce_cnt == 0)
		k1 = plane_intersection(plane, ray, ray_arr[i]);
/* 	else
		return ; */
    if (k1 < depth_buf[i] && k1 > 0.01)
    {
        depth_buf[i] = k1;
        index_buf[i] = index;
    }
}
