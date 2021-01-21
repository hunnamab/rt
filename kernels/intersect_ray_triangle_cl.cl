#include "kernel.h"

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
                                float3 camera_start, \
                                __global float *depth_buf, \
                                t_triangle triangle, \
                                __global int *index_buf, \
                                int index)
{
    int i = get_global_id(0);
    float res;
	res = triangle_intersection(triangle, camera_start, ray_arr[i]);
	if (res > 0 && res < depth_buf[i])
	{
		depth_buf[i] = res;
		index_buf[i] = index;
		return ;
	}
}
