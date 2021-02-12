#include "kernel.h"

int cut(float3 point, __global t_cutting_surface *cs, int cs_nmb)
{
    int i;
    float result;
    i = 0;
    while(i < cs_nmb)
    {
        if (cs[i].type == PLANE)
        {
            result = cs[i].param1.x * point.x + cs[i].param1.y * point.y + cs[i].param1.z * point.z + cs[i].param3;
            if (result >= 0)
                return (0);
        }
		if(cs[i].type == SPHERE)
		{
			float3 buf;
			buf = point - cs[i].param1;
			result = length(buf);
			if (result >= cs[i].param3 && !cs[i].is_negative)
				return(0);
			if(result <= cs[i].param3 && cs[i].is_negative)
				return(0);
		}
        i++;
    }
    return (1);
}

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
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_plane plane, \
                                __global int *index_buf, \
                                int index, int bounce_cnt, \
								__global t_cutting_surface *cs, \
								int cs_nmb, __global t_material *material_buf, \
								int is_refractive, __global float3 *normal_buf, \
								int refraction, __global int *exception_buf)
{
    int i = get_global_id(0);
    
	float k1;
	float3 ray;
	
	if (exception_buf[i] == index)
	{
		exception_buf[i] = -1;
		return ;
	}
    if (bounce_cnt > 0 || is_refractive)
    	camera_start[i] = camera_start[i] + ray_arr[i] * 0.00001f;
	if ((bounce_cnt == 0 && !is_refractive) || (bounce_cnt == 0 && is_refractive && material_buf[i].refraction > 0.0) || material_buf[i].reflection > 0.0)
		k1 = plane_intersection(plane, camera_start[i], ray_arr[i]);
	else
		return ;
    if (k1 < depth_buf[i] && k1 > 0.01)
    {
		float3 intersection_point;
        intersection_point = ray_arr[i] * k1;
        intersection_point = intersection_point + camera_start[i];
        if (cut(intersection_point, cs, cs_nmb))
        {
			depth_buf[i] = k1;
			index_buf[i] = index;
		}
    }
}
