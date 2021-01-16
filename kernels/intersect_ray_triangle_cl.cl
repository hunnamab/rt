#include "kernel.h"

/* float triangle_cl()
{
	float3 edge1;
	float3 edge2;
    float3 vec1;
	float3 vec2;
	float3 vec3;
    float det;
    float uv1;
	float uv2;

    edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	vec1 = cross(ray_arr[i], edge2);
	det = dot(edge1, vec1);
	if (det < 1e-8 && det > -1e-8)
		return ;
	det = 1 / det;
	vec2 = camera_start[0] - vertex0;
	uv1 = dot(vec2, vec1) * det;
	if (uv1 < 0 || uv1 > 1)
		return ;
	vec3 = cross(vec2, edge1);
	uv2 = dot(ray_arr[i], vec3) * det;
	if (uv2 < 0 || uv1 + uv2 > 1)
		return ;
	float res;
	res = dot(edge2, vec3) * det;
} */

__kernel void intersect_ray_triangle_cl(__global float3 *ray_arr, \
                                __global float3 *camera_start, \
                                __global float *depth_buf, \
                                float3 vertex0, \
								float3 vertex1, \
								float3 vertex2, \
                                __global int *index_buf, \
                                int index)
{
    int i = get_global_id(0);
    
    float3 edge1;
	float3 edge2;
    float3 vec1;
	float3 vec2;
	float3 vec3;
    float det;
    float uv1;
	float uv2;

    edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	vec1 = cross(ray_arr[i], edge2);
	det = dot(edge1, vec1);
	if (det < 1e-8 && det > -1e-8)
		return ;
	det = 1 / det;
	vec2 = camera_start[0] - vertex0;
	uv1 = dot(vec2, vec1) * det;
	if (uv1 < 0 || uv1 > 1)
		return ;
	vec3 = cross(vec2, edge1);
	uv2 = dot(ray_arr[i], vec3) * det;
	if (uv2 < 0 || uv1 + uv2 > 1)
		return ;
	float res;
	res = dot(edge2, vec3) * det;
	if (res > 0 && res < depth_buf[i])
	{
		depth_buf[i] = res;
		index_buf[i] = index;
		return ;
	}
}
