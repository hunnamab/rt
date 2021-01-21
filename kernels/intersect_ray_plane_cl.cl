#include "kernel.h"

float plane_intersection(t_plane plane, float3 ray_start, float3 ray_dir)
{
	float k1;
	float k2;

    if ((dot(ray_dir, plane.normal)) == 0)
		return (0);
	k1 = dot(ray_start, plane.normal) + plane.d;
	k2 = dot(ray_dir, plane.normal);
	if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
		return (0);
	k1 = -k1 / k2;
	return (k1);
}

__kernel void intersect_ray_plane_cl(__global float3 *ray_arr, \
                                float3 camera_start, \
                                __global float *depth_buf, \
                                t_plane plane, \
                                __global int *index_buf, \
                                int index)
{
    int i = get_global_id(0);
    
	float k1;
	k1 = plane_intersection(plane, camera_start, ray_arr[i]);
    
    if (k1 < depth_buf[i] && k1 > 0)
    {
        depth_buf[i] = k1;
        index_buf[i] = index;
    }
}
