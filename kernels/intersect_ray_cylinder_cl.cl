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

float cylinder_intersection(t_cylinder cyl, float3 ray_start, float3 ray_dir)
{
	float t1;
    float t2;
    float b;
    float c;

    float3 dist = ray_start - cyl.position;
	float a = dot(ray_dir, cyl.vec);
	a = dot(ray_dir, ray_dir) - a * a;
    b = 2 * (dot(ray_dir, dist) - dot(ray_dir, cyl.vec) * \
		dot(dist, cyl.vec));
    c = dot(dist, cyl.vec);
	c = dot(dist, dist) - c * c - cyl.radius * cyl.radius;
    c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
	}
	return (0);
}

__kernel void intersect_ray_cylinder_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_cylinder cyl, \
                                __global int *index_buf, \
                                int index, int bounce_cnt, \
								__global t_cutting_surface *cs, \
								int cs_nmb, __global t_material *material_buf)
{
    int i = get_global_id(0);
	float result;
	float3 ray;
    ray = camera_start[i] + ray_arr[i] * 0.00001f;
	if (bounce_cnt == 0 || material_buf[i].reflection > 0.0)
		result = cylinder_intersection(cyl, ray, ray_arr[i]);
	else
		return ;
	if (result > 0.01 && result < depth_buf[i])
    {
		float3 intersection_point;
        intersection_point = ray_arr[i] * result;
        intersection_point = intersection_point + camera_start[i];
        if (cut(intersection_point, cs, cs_nmb))
        {
			depth_buf[i] = result;
			index_buf[i] = index;
		}
	}
}
