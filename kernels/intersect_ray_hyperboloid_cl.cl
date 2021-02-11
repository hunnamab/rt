#include "kernel.h"

/* 
int cut(float3 point, __global t_cutting_surface *cs, int cs_nmb)
{
    int i;
    float result;
    i = 0;
    while(i < cs_nmb)
    {
        if (cs[i].type == PLANE)
        {
            result = cs[i].primitive.plane.normal.x * point.x + cs[i].primitive.plane.normal.y * point.y + cs[i].primitive.plane.normal.z * point.z + cs[i].primitive.plane.d;
            if (result >= 0)
                return (0);
        }
		if(cs[i].type == SPHERE)
		{
			float3 buf;
			buf = point - cs[i].primitive.sphere.center;
			result = length(buf);
			if (result >= cs[i].primitive.sphere.radius && !cs[i].is_negative)
				return(0);
			if(result <= cs[i].primitive.sphere.radius && cs[i].is_negative)
				return(0);
		}
        i++;
    }
    return (1);
}
 */
float hyperboloid_intersection(t_hyperboloid hyper, float3 ray_start, float3 ray_dir)
{
 	float k1;
    float k2;
    float k3;
	float a = hyper.a;
	float b = hyper.b;
	float c = hyper.c;
	float3 co = ray_start - hyper.center;
    k1 = ray_dir.x * ray_dir.x / (a * a) + ray_dir.y * ray_dir.y / (b * b) - ray_dir.z * ray_dir.z / (c * c);
    k2 = 2 * co.x * ray_dir.x / (a * a) + 2 * co.y * ray_dir.y / (b * b) - 2 * co.z * ray_dir.z / (c * c);
    k3 = co.x * co.x / (a * a) + co.y * co.y / (b * b) - co.z * co.z / (c * c) + 1.0f;
    float d = k2 * k2 - 4 * k1 * k3;
    if (d >= 0)
    {
        float t1 = (-k2 + sqrt(d)) / (2 * k1);
        float t2 = (-k2 - sqrt(d)) / (2 * k1);
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            return (t1);
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            return (t2);
        if (t2 == t1 && t2 >= 0)
            return (t2);
    }
    return (0);
}

__kernel  void    intersect_ray_hyperboloid(__global float3 *ray_arr, \
                                        __global float3 *camera_start, \
                                        t_hyperboloid hyper, __global float *depth_buf, \
                                        __global int *index_buf, int index, \
										int bounce_cnt, __global t_cutting_surface *cs, \
										int cs_nmb, __global t_material *material_buf)
{
    int i = get_global_id(0);
    float result;
    float3 ray;
    ray = camera_start[i] + ray_arr[i] * 0.00001f;
	if (bounce_cnt == 0 || material_buf[i].reflection > 0.0)
		result = hyperboloid_intersection(hyper, ray, ray_arr[i]);
	else
		return ;
    if (i == 0)
        printf("sizeof(hyperbaloid DEVICE%lu\n", sizeof(t_hyperboloid));
    if (result > 0.01 && result < depth_buf[i])
    {
		float3 intersection_point;
        intersection_point = ray_arr[i] * result;
        intersection_point = intersection_point + camera_start[i];
     //   if (cut(intersection_point, cs, cs_nmb))
     //   {
			depth_buf[i] = result;
			index_buf[i] = index;
//		}
    }
}
