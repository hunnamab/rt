__kernel void intersect_ray_cone_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float3 *position, \
                                __global float *depth_buf, \
                                __global float3 *vector, \
                                __global float *angle, \
                                __global int *index_buf, \
                                __global int *index)
{
    int i = get_global_id(0);
    float t1;
    float t2;
    float b;
    float c;
    float ang = *angle;
    float3 vec = *vector;
    float3 dist = camera_start[0] - position[0];
	float a = dot(ray_arr[i], vec);
	a = dot(ray_arr[i], ray_arr[i]) - (1 + ang * ang) * a * a;
    b = 2 * (dot(ray_arr[i], dist) - (1 + ang * ang) * \
		dot(ray_arr[i], vec) * dot(dist, vec));
    c = dot(dist, vec);
	c = dot(dist, dist) - (1 + ang * ang) * c * c;
	c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        depth_buf[i] = t1 < t2 ? t1 : t2;
        index_buf[i] = index;
	}
}
