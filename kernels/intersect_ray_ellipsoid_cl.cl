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
	float3			center;
}					t_box;

typedef	union			primitive
{
	t_cylinder			cylinder;
	t_cone				cone;
	t_sphere			sphere;
	t_plane				plane;
	t_triangle			triangle;
	t_ellipsoid			ellipsoid;
	t_box				box;
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
	int					color_disrupt;
	int 				type;
	int					texture_id;
	int 				texture_size;
	int					texture_width;
	int					texture_height;
	int					l_size;
}						t_object_d;

float ellipsoid_intersection(t_ellipsoid el, float3 ray_start, float3 ray_dir)
{
    float k1;
    float k2;
    float k3;
    float3 dir;
    float a2;
    float b2;
    float c2;

    a2 = el.abc.x * el.abc.x;
    b2 = el.abc.y * el.abc.y;
    c2 = el.abc.z * el.abc.z;
    dir = ray_dir;
    k1 = dir.x * dir.x * b2 * c2;
    k1 += dir.y * dir.y * a2 * c2; 
    k1 += dir.z * dir.z * a2 * b2;
    k2 = ray_start.x * dir.x * b2 * c2 * 2;
    k2 += ray_start.y * dir.y * a2 * c2 * 2;
    k2 += ray_start.z * dir.z * b2 * a2 * 2;
    k3 = ray_start.x * ray_start.x * b2 * c2;
    k3 += ray_start.z * ray_start.z * a2 * b2;
    k3 += ray_start.y * ray_start.y * a2 * c2;
    k3 -= a2 * b2 * c2;
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

__kernel  void    intersect_ray_ellipsoid(__global float3 *ray_arr, float3 camera_start, t_ellipsoid el, __global float *depth_buf, __global int *index_buf, int index)
{
    int i = get_global_id(0);
    float result;
    result = ellipsoid_intersection(el, camera_start, ray_arr[i]);
    /* float k1;
    float k2;
    float k3;
    float3 dir;
    float a2;
    float b2;
    float c2;

    a2 = el.abc.x * el.abc.x;
    b2 = el.abc.y * el.abc.y;
    c2 = el.abc.z * el.abc.z;
    dir = ray_arr[i];
    k1 = dir.x * dir.x * b2 * c2;
    k1 += dir.y * dir.y * a2 * c2; 
    k1 += dir.z * dir.z * a2 * b2;
    k2 = camera_start.x * dir.x * b2 * c2 * 2;
    k2 += camera_start.y * dir.y * a2 * c2 * 2;
    k2 += camera_start.z * dir.z * b2 * a2 * 2;
    k3 = camera_start.x * camera_start.x * b2 * c2;
    k3 += camera_start.z * camera_start.z * a2 * b2;
    k3 += camera_start.y * camera_start.y * a2 * c2;
    k3 -= a2 * b2 * c2;
    float d = k2 * k2 - 4 * k1 * k3;
    if (d >= 0)
    {
        float t1 = (-k2 + sqrt(d)) / (2 * k1);
        float t2 = (-k2 - sqrt(d)) / (2 * k1);
        float result = 0;
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            result = t1;
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            result = t2;
        if (t2 == t1 && t2 >= 0)
            result = t2;
        if (result > 0 && result < depth_buf[i])
        {
            depth_buf[i] = result;
            index_buf[i] = index;
        }
    } */
    if (result > 0.01 && result < depth_buf[i])
    {
        depth_buf[i] = result;
        index_buf[i] = index;
    }
}