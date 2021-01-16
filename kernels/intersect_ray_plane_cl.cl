#include "kernel.h"

__kernel void intersect_ray_plane_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                float3 normal, \
								float d, \
                                __global int *index_buf, \
                                int index)
{
    int i = get_global_id(0);
    
	float k1;
	float k2;

    if ((dot(ray_arr[i], normal)) == 0)
		return ;
	k1 = dot(camera_start[0], normal) + d;
	k2 = dot(ray_arr[i], normal);
	if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
		return ;
	k1 = -k1 / k2;
    if (k1 < depth_buf[i] && k1 > 0)
    {
        depth_buf[i] = -k1 / k2;
        index_buf[i] = index;
    }
}
