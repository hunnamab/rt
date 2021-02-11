//#include "kernel.h"

__kernel void get_ray_arr(__global float3* ray_arr, \
                        float3 camera_start, \
                        __global float3* viewport, \
                        const unsigned int count)
{
    int i = get_global_id(0);
    ray_arr[i] = viewport[i] - camera_start;
}
