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

typedef	struct		s_color
{
	unsigned int	red;
	unsigned int	green;
	unsigned int	blue;
	unsigned int	alpha;
}					t_color;

typedef	struct		s_sphere
{
	float3			center;
	float			radius;
}					t_sphere;

typedef	struct		s_object3d_d
{
	t_sphere		sphere;
/* 	t_plane			plane;
	t_triangle		triangle;
	t_cone			cone;
	t_cylinder		cylinder;*/
	int 			type;
	t_color			color;
	float			specular;
}					t_object_d;

void get_normal_sphere(t_object_d *obj, \
                        float3 *ray_buf, \
                        int *index_buf, \
                        float3 *normal_buf, \
                        float3 *intersection_buf, \
                        int index)
{
	float l;
	int j;

	j = index_buf[index];
	printf("%d ", obj[j].sphere.center);
	normal_buf[index] = intersection_buf[index] - obj[j].sphere.center;
	l = length(normal_buf[index]);
	normal_buf[index] = native_divide(normal_buf[index], l);
    if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
	{
		normal_buf[index].x = normal_buf[index].x * -1;
		normal_buf[index].y = normal_buf[index].y * -1;
		normal_buf[index].z = normal_buf[index].z * -1;
	}
}

__kernel void get_normal_buf_cl(__global t_object_d *obj, \
                                __global float3 *ray_buf, \
                                __global int *index_buf, \
                                __global float3 *normal_buf, \
                                __global float3 *intersection_buf)
{
    int i = get_global_id(0);
	int j = index_buf[i];
	//float l;
	
	if (j != -1)
	{
		printf("j = %d, obj[j].type = %d\n", j, obj[j].type);
		//if (obj[j].type == 0)
		//{
			// normal_buf[index] = intersection_buf[index] - obj[j].sphere.center;
			// l = length(normal_buf[index]);
			// normal_buf[index] = native_divide(normal_buf[index], l);
			// if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
			// {
			// 	normal_buf[index].x = normal_buf[index].x * -1;
			// 	normal_buf[index].y = normal_buf[index].y * -1;
			// 	normal_buf[index].z = normal_buf[index].z * -1;
			// }
		//}
			get_normal_sphere(&obj, &ray_buf, &index_buf, &normal_buf, &intersection_buf, i);
	}
}
