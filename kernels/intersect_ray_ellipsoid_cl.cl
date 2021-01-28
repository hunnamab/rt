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

float ellipsoid_intersection(t_ellipsoid el, float3 ray_start, float3 ray_dir)
{
    float k1;
    float k2;
    float k3;
	float dist;
 
	dist = distance(el.center2, el.center1);
    float3 el_dir = ray_start - el.center1;
    float3 center_norm = normalize((el.center2 - el.center1) / dist);
    k1 = 4 * pow(el.radius, 2) * dot(ray_dir, ray_dir) - 4 * pow(dist, 2) * pow(dot(ray_dir, center_norm), 2);
    k2 = 8 * pow(el.radius, 2) * dot(ray_dir, el_dir) - 4 * dot(ray_dir, center_norm) * dist * (pow(el.radius, 2) + 2 * dot(el_dir, center_norm) * dist - dist);
    k3 = 4 * pow(el.radius, 2) * dot(el_dir, el_dir) - pow((pow(el.radius, 2) + 2 * dot(el_dir, center_norm) * dist - dist), 2);
    float d = k2 * k2 - 4 * k1 * k3;
    if (d >= 0)
    {
        float t1 = (-k2 + sqrt(d)) / (2 * k1);
        float t2 = (-k2 - sqrt(d)) / (2 * k1);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
    }
    return (0);
}

__kernel  void    intersect_ray_ellipsoid(__global float3 *ray_arr, \
                                        __global float3 *camera_start, \
                                        t_ellipsoid el, __global float *depth_buf, \
                                        __global int *index_buf, int index, \
										float reflection, int bounce_cnt)
{
    int i = get_global_id(0);
    float result;
    float3 ray;
    ray = camera_start[i] + ray_arr[i] + 0.001f;
	//if (reflection > 0.001f || bounce_cnt == 0)
    	result = ellipsoid_intersection(el, ray, ray_arr[i]);
/* 	else
		return ; */
    if (result > 0.01 && result < depth_buf[i])
    {
        depth_buf[i] = result;
        index_buf[i] = index;
    }
}
