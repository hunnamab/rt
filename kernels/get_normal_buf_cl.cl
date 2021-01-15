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

typedef struct 		s_color
{
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
	unsigned char	alpha;
}					t_color;

typedef struct 		s_sphere
{
	float3			center;
	float			radius;
}					t_sphere;

typedef struct 		s_plane
{
	float3			normal;
	float3			point;
	float			d;
}					t_plane;

typedef struct 		s_cylinder
{
	float3			position;
	float3			vec;
	float			radius;
}					t_cylinder;

typedef struct 		s_cone
{
	float3			position;
	float3			vec;
	float			angle;
}					t_cone;

typedef struct 		s_triangle
{
	float3			vertex[3];
	float3			normal;
}					t_triangle;

typedef	union			primitive
{
	t_sphere			sphere;
	t_plane				plane;
	t_triangle			triangle;
	t_cone				cone;
	t_cylinder			cylinder;
}						t_primitive;

typedef struct 			 s_object3d_d
{
	t_primitive			primitive;
	t_color				color;
	float				specular;
	int 				type;
}						t_object_d;

void get_normal_cylinder(__global t_object_d *obj, \
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

    buf1 = camera_position - obj[0].primitive.cylinder.position;
    m = dot(ray_buf[0],  obj[0].primitive.cylinder.vec) * depth_buf[0] + dot(buf1, obj[0].primitive.cylinder.vec);
    buf1.x = ray_buf[0].x * depth_buf[0];
	buf1.y = ray_buf[0].y * depth_buf[0];
	buf1.z = ray_buf[0].z * depth_buf[0];
	p = camera_position + buf1;
	buf1 = p - obj[0].primitive.cylinder.position;
	buf2.x = obj[0].primitive.cylinder.vec.x * m;
	buf2.y = obj[0].primitive.cylinder.vec.y * m;
	buf2.z = obj[0].primitive.cylinder.vec.z * m;
	normal_buf[0] = buf1 - buf2;
	normal_buf[0] = native_divide(normal_buf[0], length(normal_buf[0]));
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
    {
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
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
    buf.x = obj[0].primitive.cone.vec.x * m;
	buf.y = obj[0].primitive.cone.vec.y * m;
	buf.z = obj[0].primitive.cone.vec.z * m;
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
								float3 camera_position)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	float l;
	if (i == 0)	
	{
		printf("t_object_d device = %lu", sizeof(t_object_d));
		// printf("j = %d, obj[j].specular = %f\n", j, obj[j].specular);
		// printf("obj[j].radius = %f center = (%f,%f,%f)\n", obj[j].sphere.radius, obj[j].sphere.center.x,obj[j].sphere.center.y,obj[j].sphere.center.z);
		//printf("j = %d, obj[j].type = %d\n", j, obj[j].type);
		// printf("%f", obj.sphere.radius);
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
			get_normal_cone(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], camera_position, &depth_buf[i]);
		else if (obj[j].type == CYLINDER)
			get_normal_cylinder(&obj[j], &ray_buf[i], &index_buf[i], &normal_buf[i], &intersection_buf[i], camera_position, &depth_buf[i]);
	}
}
