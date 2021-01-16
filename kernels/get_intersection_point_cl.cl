#include "kernel.h"

__kernel void get_intersection_point(__global float3 *intersection_buf, \
                                    __global float3 *ray_buf, \
                                    __global float *depth_buf, \
                                    float3 camera, \
                                    __global int *index_buf)
 {
    int i = get_global_id(0);

    if (index_buf[i] != -1)
    {
        intersection_buf[i] = ray_buf[i] * depth_buf[i];
        intersection_buf[i] = intersection_buf[i] + camera;
    }
    else
    {
        intersection_buf[i].x = 0;
        intersection_buf[i].y = 0;
        intersection_buf[i].z = 0;
    }
}
