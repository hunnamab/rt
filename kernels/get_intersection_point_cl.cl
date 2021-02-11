//#include "kernel.h"

__kernel void get_intersection_point(__global float3 *intersection_buf, \
                                    __global float3 *ray_buf, \
                                    __global float *depth_buf, \
                                    __global float3 *camera, \
                                    __global int *index_buf)
 {
    int i = get_global_id(0);
	float3 buf;
    if (index_buf[i] != -1)
    {
		buf = camera[i];
		intersection_buf[i].x = ray_buf[i].x * depth_buf[i];
		intersection_buf[i].y = ray_buf[i].y * depth_buf[i];
		intersection_buf[i].z = ray_buf[i].z * depth_buf[i];
		intersection_buf[i] = intersection_buf[i] + buf;
    }
    else
    {
        intersection_buf[i].x = 0.0;
        intersection_buf[i].y = 0.0;
        intersection_buf[i].z = 0.0;
    }
}
    