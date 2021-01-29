
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

void  get_normal_ellipsoid(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float dist = distance(obj[0].primitive.ellipsoid.center2, obj[0].primitive.ellipsoid.center1);
    float3 el_dir = camera_position - obj[0].primitive.ellipsoid.center1;
	float radius = obj[0].primitive.ellipsoid.radius;
    float3 center_norm = normalize((obj[0].primitive.ellipsoid.center2 - obj[0].primitive.ellipsoid.center1) / dist);
 /*   float k1 = 4 * pow(radius, 2) * dot(ray_buf[0], ray_buf[0]) - 4 * pow(dist, 2) * pow(dot(ray_buf[0], center_norm), 2);
    float k2 = 8 * pow(radius, 2) * dot(ray_buf[0], el_dir) - 4 * dot(ray_buf[0], center_norm) * dist * (pow(radius, 2) + 2 * dot(el_dir, center_norm) * dist - dist);
    float k3 = 4 * pow(radius, 2) * dot(el_dir, el_dir) - pow((pow(radius, 2) + 2 * dot(el_dir, center_norm) * dist - dist), 2); */
/* 	float a = k1;
	float b = k2;
	float3 Cmid = obj[0].primitive.ellipsoid.center2 + (center_norm * dist / 2);
	float3 R = intersection_buf[0] - Cmid;
	normal_buf[0] = normalize(R - center_norm * (1 - ((b * b) / (a * a)) * dot(R, center_norm))); */
	float3 center;
	center.x = obj[0].primitive.ellipsoid.center1.x + center_norm.x * dist / 2;
	center.y = obj[0].primitive.ellipsoid.center1.y + center_norm.y * dist / 2;
	center.z = obj[0].primitive.ellipsoid.center1.z + center_norm.z * dist / 2;
	//float radius = obj[0].primitive.ellipsoid.radius;
	float3 intersection = intersection_buf[0];
	float3 normal;
	normal.x = intersection.x - center.x;
	normal.y = intersection.y - center.y;
	normal.z = intersection.z - center.z;
	normal.x *= 2 / (pow(radius,2));
	normal.y *= 2 / (pow(radius,2));
	normal.z *= 2 / (pow(radius,2));
	normal_buf[0] = normalize(normal);
}

void  get_normal_cylinder(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
						__global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float	m;
    float3 buf1;
	float3 buf2;
	float3 p;
	float3 normal;
    buf1 = camera_position - obj[0].primitive.cylinder.position;
    m = dot(ray_buf[0],  obj[0].primitive.cylinder.vec) * depth_buf[0] + dot(buf1, obj[0].primitive.cylinder.vec);
    buf1 = ray_buf[0] * depth_buf[0];
	p = camera_position + buf1;
	buf1 = p - obj[0].primitive.cylinder.position;
	buf2 = obj[0].primitive.cylinder.vec * m;
	normal_buf[0] = buf1 - buf2;
 	normal_buf[0] /= length(normal_buf[0]);
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
		normal_buf[0] *= -1.0f;
}

void get_normal_cone(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
                        __global float3 *normal_buf, \
                        __global float3 *intersection_buf, \
						float3 camera_position, \
						__global float *depth_buf)
{
	float	m;
    float3 buf;
	float n;
    buf = camera_position - obj[0].primitive.cone.position;
    m = dot(ray_buf[0],  obj[0].primitive.cone.vec) * depth_buf[0] + dot(buf, obj[0].primitive.cone.vec);
    buf = obj[0].primitive.cone.vec * m;
	n = 1 +  obj[0].primitive.cone.angle * obj[0].primitive.cone.angle;
    normal_buf[0].x = buf.x * n;
	normal_buf[0].y = buf.y * n;
	normal_buf[0].z = buf.z * n;
    buf = intersection_buf[0] - obj[0].primitive.cone.position;
    normal_buf[0] = buf - normal_buf[0];
    normal_buf[0] = native_divide(normal_buf[0], length(normal_buf[0]));
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
    {
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_sphere(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global int *index_buf, \
                        __global float3 *normal_buf, \
                        __global float3 *intersection_buf)
{
	float l;

	normal_buf[0] = intersection_buf[0] - obj[0].primitive.sphere.center;
	l = length(normal_buf[0]);
	normal_buf[0] = native_divide(normal_buf[0], l);
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_triangle(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global float3 *normal_buf)
{
	normal_buf[0] = obj[0].primitive.triangle.normal;
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

void get_normal_plane(__global t_object_d *obj, \
                        __global float3 *ray_buf, \
                        __global float3 *normal_buf)
{
	normal_buf[0] = obj[0].primitive.plane.normal;
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
}

__kernel void get_normal_buf_cl(__global t_object_d *obj, \
                                __global float3 *ray_buf, \
                                __global int *index_buf, \
                                __global float3 *normal_buf, \
                                __global float3 *intersection_buf, \
								__global float *depth_buf, \
								float3 camera_position, int bounce_cnt)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	float l;
	float3 buf_camera;
	if (bounce_cnt == 0)
		buf_camera = camera_position;
	else
	{
		buf_camera.x = intersection_buf[i].x + ray_buf[i].x * -1 * depth_buf[i];
		buf_camera.y = intersection_buf[i].y + ray_buf[i].y * -1 * depth_buf[i];
		buf_camera.z = intersection_buf[i].z + ray_buf[i].z * -1 * depth_buf[i];
	}
	if (j != -1)
	{
		if (obj[j].type == SPHERE)
			get_normal_sphere(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i]);
		else if (obj[j].type == PLANE)
			get_normal_plane(&obj[j], &ray_buf[i], &normal_buf[i]);
		else if (obj[j].type == TRIANGLE)
			get_normal_triangle(&obj[j], &ray_buf[i], &normal_buf[i]);
		else if (obj[j].type == CONE)
			get_normal_cone(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else if (obj[j].type == CYLINDER)
			get_normal_cylinder(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
		else if (obj[j].type == ELLIPSOID)
			get_normal_ellipsoid(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], buf_camera, &depth_buf[i]);
	}
	else
		normal_buf[i] = 0;
}
