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

typedef	struct		 	s_cutting_surface
{
	int					type;
	int					is_negative;
	t_primitive			primitive;
}						t_cutting_surface;

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

int cut(float3 point, __global t_cutting_surface *cs, int cs_nmb)
{
    int i;
    float result;
    i = 0;
    while(i < cs_nmb)
    {
        if (cs[i].type == PLANE)
        {
            result = cs[i].primitive.plane.normal.x * point.x + cs[i].primitive.plane.normal.y * point.y + cs[i].primitive.plane.normal.z * point.z + cs[i].primitive.plane.d;
            if (result >= 0)
                return (0);
        }
		if(cs[i].type == SPHERE)
		{
			float3 buf;
			buf = point - cs[i].primitive.sphere.center;
			result = length(buf);
			if (result >= cs[i].primitive.sphere.radius && !cs[i].is_negative)
				return(0);
			if(result <= cs[i].primitive.sphere.radius && cs[i].is_negative)
				return(0);
		}
        i++;
    }
    return (1);
}

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

__kernel void intersect_ray_cone_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_cone cone, \
                                __global int *index_buf, \
                                int index, float reflection, int bounce_cnt, __global t_cutting_surface *cs, \
								int cs_nmb)
{
    int i = get_global_id(0);
	float result = 0;
	float3 ray;
    ray = camera_start[i] + ray_arr[i] + 0.001f;
	//if (reflection > 0.001f || bounce_cnt == 0)
		result = cone_intersection(cone, ray, ray_arr[i]);
/* 	else
		return ; */
	if (result > 0.01 && result < depth_buf[i])
    {
		 float3 intersection_point;
        intersection_point = ray_arr[i] * result;
        intersection_point = intersection_point + camera_start[i];
        if (cut(intersection_point, cs, cs_nmb))
        {
			depth_buf[i] = result;
			index_buf[i] = index;
		}
    }
}
