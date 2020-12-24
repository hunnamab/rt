__kernel void intersect_ray_cylinder_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float3 *position, \
                                __global float *depth_buf, \
                                __global float3 *vector, \
                                __global float *radius, \
                                __global int *index_buf, \
                                __global int *index)
{
    int i = get_global_id(0);
    float t1;
    float t2;
    float b;
    float c;
    float rad = *radius;
    float3 vec = *vector;
    float3 dist = camera_start[0] - position[0];
	float a = dot(ray_arr[i], vec);
	a = dot(ray_arr[i], ray_arr[i]) - a * a;
    b = 2 * (dot(ray_arr[i], dist) - dot(ray_arr[i], vec) * \
		dot(dist, vec));
    c = dot(dist, vec);
	c = dot(dist, dist) - c * c - rad * rad;
    c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if (t1 < depth_buf[i] || t2 < depth_buf[i])
        {
            depth_buf[i] = t1 < t2 ? t1 : t2;
            index_buf[i] = index;
        }
	}
}
