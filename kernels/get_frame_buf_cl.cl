//#include "kernel.h"

enum light_type{
	POINT,
	AMBIENT,
	DIRECTIONAL
};

typedef	struct		s_light
{
	float3			position;
	float3			direction;
	float			intensity;
	int				type;
}					t_light;

typedef struct 		s_color
{
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
	unsigned char	alpha;
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

typedef	union			primitive
{
	t_cylinder			cylinder;
	t_cone				cone;
	t_sphere			sphere;
	t_plane				plane;
	t_triangle			triangle;
	t_ellipsoid			ellipsoid;
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
	PARABOLOID
};

typedef struct 			s_object3d_d
{
	t_primitive			primitive;
	float3				rotation;
	t_color				color;
	float				specular;
	float				roughness;
	float				refraction;
	int					color_disrupt;
	int 				type;
	int					texture_id;
}						t_object_d;

__kernel void get_frame_buf_cl(__global t_color *frame_buf, \
                            __global float3 *ray_buf, \
                            __global float3 *intersection_buf, \
                            __global float3 *normal_buf, \
                            __global int *index_buf, \
                            __global t_material *matetial_buf, \
                            __global t_object_d *obj, \
                            __global t_light *light, \
                            int light_nmb)
{
    int i = get_global_id(0);
	if (i == 0)
	{
		printf("light_nmb = %d\nlight_intensity = %f\nlight_type = %d\n", light_nmb, light[1].intensity, light[1].type);
		printf("obj type = %d\n", obj[0].type);
		printf("sizeof light (kernel) = %lu\n", sizeof(t_light));
	}
}
