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
	float			radius;
	float3			center;
}					t_sphere;

typedef struct 		s_plane
{
	float			d;
	float3			normal;
	float3			point;
}					t_plane;

typedef struct 		s_cylinder
{
	float			radius;
	float3			position;
	float3			vec;
}					t_cylinder;

typedef struct 		s_cone
{
	float			angle;
	float3			position;
	float3			vec;
}					t_cone;

typedef struct 		s_triangle
{
	float3			normal;
	float3			vertex[3];
}					t_triangle;

typedef struct 			 s_object3d_d
{
	int 				type;;
	float				specular;
	t_sphere			sphere;
	t_plane				plane;
	t_triangle			triangle;
	t_cone				cone;
	t_cylinder			cylinder;
	t_color				color;
}						t_object_d;

/* void get_normal_sphere(t_object_d *obj, \
                        float3 *ray_buf, \
                        int *index_buf, \
                        float3 *normal_buf, \
                        float3 *intersection_buf, \
                        int index)
{
	float l;

	//printf("%d ", index_buf);
	//printf("%f ", obj[0].sphere.radius);
	normal_buf[0] = intersection_buf[0] - obj[0].sphere.center;
	l = length(normal_buf[0]);
	normal_buf[0] = native_divide(normal_buf[0], l);
    if (dot(ray_buf[0], normal_buf[0]) > 0.0001)
	{
		normal_buf[0].x = normal_buf[0].x * -1;
		normal_buf[0].y = normal_buf[0].y * -1;
		normal_buf[0].z = normal_buf[0].z * -1;
	}
} */

__kernel void get_normal_buf_cl(__global t_object_d *obj, \
                                __global float3 *ray_buf, \
                                __global int *index_buf, \
                                __global float3 *normal_buf, \
                                __global float3 *intersection_buf)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	float l;

	if (i == 242500)
	{
		printf("t_object_d device = %lu", sizeof(t_object_d));
		printf("j = %d, obj[j].specular = %f\n", j, obj[j].specular);
		printf("obj[j].radius = %f center = (%f,%f,%f)\n", obj[j].sphere.radius, obj[j].sphere.center.x,obj[j].sphere.center.y,obj[j].sphere.center.z);
		printf("j = %d, obj[j].type = %d\n", j, obj[j].type);
		// printf("t_object_d on device = %lu\n", sizeof(t_object_d));
		// printf("%f", obj.sphere.radius);
		// if (obj[j].type == SPHERE)
		// {
		// 	normal_buf[i] = intersection_buf[i] - obj[j].center;
		// 	l = length(normal_buf[i]);
		// 	normal_buf[i] = native_divide(normal_buf[i], l);
		// 	if (dot(ray_buf[i], normal_buf[i]) > 0.0001)
		// 	{
		// 		normal_buf[i].x = normal_buf[i].x * -1;
		// 		normal_buf[i].y = normal_buf[i].y * -1;
		// 		normal_buf[i].z = normal_buf[i].z * -1;
		// 	}
		// }
		//get_normal_sphere(obj[j], ray_buf[i], index_buf[i], normal_buf[i], intersection_buf[i], i);
	}
}
