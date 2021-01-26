//#include "kernel.h"

enum light_type{
	POINT,
	AMBIENT,
	DIRECTIONAL
};

typedef struct          s_basis
{
    float3       u;
    float3       v;
    float3       w;
}                      	t_basis;

typedef	struct		s_light
{
	float3			position;
	float3			direction;
	float			intensity;
	int				type;
}					t_light;

typedef struct 		s_color
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
}					t_material;

typedef struct  s_sphere
{
    float3      center;
    float       radius;
}               t_sphere;

typedef struct 	s_plane
{
    float3		normal;
	float3		point;
	float		d;
}				t_plane;

typedef struct 		s_cylinder
{
    float3		position;
	float3		vec;
	float		radius;
}					t_cylinder;

typedef struct 		s_cone
{
    float3		    position;
	float3		    vec;
	float			angle;
}					t_cone;

typedef struct 		s_triangle
{
    float3		vertex[3];
	float3		normal;
}					t_triangle;

typedef	struct 		s_ellipsoid
{
	float3 			abc;
	float3			center;
}					t_ellipsoid;

typedef	struct 		s_box
{
	float3 			a;
	float3			b;
}					t_box;

typedef struct		s_paraboloid
{
	float			k;
	float3			center;
}					t_paraboloid;

typedef	union			primitive
{
	t_cylinder			cylinder;
	t_cone				cone;
	t_sphere			sphere;
	t_plane				plane;
	t_triangle			triangle;
	t_ellipsoid			ellipsoid;
	t_box				box;
    t_paraboloid        paraboloid;
}						t_primitive;

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
	BOX
};

typedef struct 			s_object3d_d
{
	t_primitive			primitive;
	t_basis				basis;
	float3				rotation;
	t_color				color;
	float				specular;
	float				roughness;
	float				refraction;
	float				reflection;
	int					color_disrupt;
	int 				type;
	int					texture_id;
	int 				texture_size;
	int					texture_width;
	int					texture_height;
	int					l_size;
}						t_object_d;

float paraboloid_intersection(t_paraboloid parab, float3 ray_start, float3 ray_dir)
{
    float3 parab_dir;
    float3 dir_norm;
    float k1;
    float k2;
    float k3;

    parab_dir = ray_start - parab.center;
    dir_norm = normalize(parab.center);
    k1 = dot(ray_dir, ray_dir) - pow(dot(ray_dir, dir_norm), 2);
    k2 = (dot(ray_dir, parab_dir) - dot(ray_dir, dir_norm) * (dot(parab_dir, dir_norm) + 2 * parab.k));
    k3 = dot(parab_dir, parab_dir) - dot(parab_dir, dir_norm) * (dot(parab_dir, dir_norm) + 4 * parab.k);
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

__kernel  void    intersect_ray_paraboloid(__global float3 *ray_arr, __global float3 *camera_start, t_paraboloid parab, __global float *depth_buf, __global int *index_buf, int index)
{
    int i = get_global_id(0);
    float result;
    result = paraboloid_intersection(parab, camera_start[i], ray_arr[i]);
    if (result > 0.01 && result < depth_buf[i])
    {
        depth_buf[i] = result;
        index_buf[i] = index;
    }
}