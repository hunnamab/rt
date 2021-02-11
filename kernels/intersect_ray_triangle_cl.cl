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

float triangle_intersection(t_triangle triangle, float3 ray_start, float3 ray_dir)
{
	float3 edge1;
	float3 edge2;
    float3 vec1;
	float3 vec2;
	float3 vec3;
    float det;
    float uv1;
	float uv2;

    edge1 = triangle.vertex[1] - triangle.vertex[0];
	edge2 = triangle.vertex[2] - triangle.vertex[0];
	vec1 = cross(ray_dir, edge2);
	det = dot(edge1, vec1);
	if (det < 1e-8 && det > -1e-8)
		return (0);
	det = 1 / det;
	vec2 = ray_start - triangle.vertex[0];
	uv1 = dot(vec2, vec1) * det;
	if (uv1 < 0 || uv1 > 1)
		return (0);
	vec3 = cross(vec2, edge1);
	uv2 = dot(ray_dir, vec3) * det;
	if (uv2 < 0 || uv1 + uv2 > 1)
		return (0);
	float res;
	res = dot(edge2, vec3) * det;
	return (res);
}

__kernel void intersect_ray_triangle_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                t_triangle triangle, \
                                __global int *index_buf, \
                                int index, int bounce_cnt,\
								 __global t_cutting_surface *cs, \
								int cs_nmb, __global t_material *material_buf)
{
    int i = get_global_id(0);
    float res;
	float3 ray;
    ray = camera_start[i] + ray_arr[i] * 0.00001f;
	if (bounce_cnt == 0 || material_buf[i].reflection > 0.0)
		res = triangle_intersection(triangle, ray, ray_arr[i]);
	else
		return ;
	if (res > 0.01 && res < depth_buf[i])
	{
		float3 intersection_point;
        intersection_point = ray_arr[i] * res;
        intersection_point = intersection_point + camera_start[i];
        if (cut(intersection_point, cs, cs_nmb))
        {
			depth_buf[i] = res;
			index_buf[i] = index;
		}
	}
}
