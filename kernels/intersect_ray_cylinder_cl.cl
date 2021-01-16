#include "kernel.h"

__kernel void intersect_ray_cylinder_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float3 *position, \
                                __global float *depth_buf, \
                                float3 vector, \
                                float radius, \
                                __global int *index_buf, \
                                int index)
{
    int i = get_global_id(0);
    float t1;
    float t2;
    float b;
    float c;

    float3 dist = camera_start[0] - position[0];
	float a = dot(ray_arr[i], vector);
	a = dot(ray_arr[i], ray_arr[i]) - a * a;
    b = 2 * (dot(ray_arr[i], dist) - dot(ray_arr[i], vector) * \
		dot(dist, vector));
    c = dot(dist, vector);
	c = dot(dist, dist) - c * c - radius * radius;
    c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        float result;
        result = 0;
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
	}
}
