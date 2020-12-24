#include "rt.cuh"
extern "C"{
    #include "rt_cuda.h"
    }
__device__ float	dot(float3 v1, float3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

__device__ float3	sub(float3 v1, float3 v2)
{
    float3 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
	return (result);
}

__device__ float3	cross(float3 v1, float3 v2)
{
	float3 result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return (result);
}

__global__ void intersect_ray_sphere_c(float3 *ray_arr, \
     float3 camera_start, \
     float3 s_center, \
     float s_radius, \
     float *depth_buf, int *index_buf, int index)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    float a = dot(ray_arr[i], ray_arr[i]);
    float b;
    float c;
    float t1;
    float t2;
    float3 dist = sub(camera_start, s_center);
    b = 2 * dot(dist, ray_arr[i]);
    c = dot(dist, dist) - (s_radius * s_radius);
    c = b * b - 4 * a * c;
    if (c >= 0)
    {
        c = sqrt(c);
        t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        float result;
        result = 0;
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            result = t1;
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            result = t2;
        if (t2 == t1 && t2 >= 0)
            result = t2;
        if (result > 0 && result < depth_buf[i])
        {
            depth_buf[i] = result;
            index_buf[i] = index;
        }
    }
}
__global__ void intersect_ray_triangle_c(float3 *ray_arr, float3 camera_start,\
    float *depth_buf, float3 *vertex, int *index_buf, int index)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    float3 edge[2];
    float3 vec[3];
    float det;
    float uv[2];

    edge[0] = sub(vertex[1],vertex[0]);
    edge[1] = sub(vertex[2],vertex[0]);
    vec[0] = cross(ray_arr[i], edge[1]);
    det = dot(edge[0], vec[0]);
    if (det < 1e-8 && det > -1e-8)
        return ;
    det = 1 / det;
    vec[1] = sub(camera_start,vertex[0]);
    uv[0] = dot(vec[1], vec[0]) * det;
    if (uv[0] < 0 || uv[0] > 1)
        return ;
    vec[2] = cross(vec[1], edge[0]);
    uv[1] = dot(ray_arr[i], vec[2]) * det;
    if (uv[1] < 0 || uv[0] + uv[1] > 1)
        return ;
    float res;
    res = dot(edge[1], vec[2]) * det;
    if (res > 0 && res < depth_buf[i])
    {
        depth_buf[i] = res;
        index_buf[i] = index;
        return ;
    }
}

__global__ void intersect_ray_cone_c(float3 *ray_arr, float3 camera_start, float3 position, \
    float3 vector, float angle, float *depth_buf, int *index_buf, int index)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    float t1;
    float t2;
    float b;
    float c;
    float ang = angle;
    float3 vec = vector;
    float3 dist = sub(camera_start, position);
    float a = dot(ray_arr[i], vec);
    a = dot(ray_arr[i], ray_arr[i]) - (1 + ang * ang) * a * a;
    b = 2 * (dot(ray_arr[i], dist) - (1 + ang * ang) * \
    dot(ray_arr[i], vec) * dot(dist, vec));
    c = dot(dist, vec);
    c = dot(dist, dist) - (1 + ang * ang) * c * c;
    c = b * b - 4 * a * c;
    if (c >= 0)
    {
        c = sqrt(c);
        t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        float result;
        result = 0;
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            result = t1;
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            result = t2;
        if (t2 == t1 && t2 >= 0)
            result = t2;
        if (result > 0 && result < depth_buf[i])
        {
            depth_buf[i] = result;
            index_buf[i] = index;
        }
    }
}

__global__ void intersect_ray_plane_c(float3 *ray_arr, \
     float3 camera_start, \
     float *depth_buf, \
     float3 normal, \
     float d, \
     int *index_buf, \
     int index)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    float k1;
    float k2;

    if ((dot(ray_arr[i], normal)) == 0)
        return ;
    k1 = dot(camera_start, normal) + d;
    k2 = dot(ray_arr[i], normal);
    if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
        return ;
    k1 = -k1 / k2;
    if(k1 < depth_buf[i] && k1 > 0)
    {
        depth_buf[i] = -k1 / k2;
        index_buf[i] = index;
    }
}

__global__ void intersect_ray_cylinder_c(float3 *ray_arr, float3 camera_start, \
    float3 position, float *depth_buf, float3 vector, \
    float radius, int *index_buf, int index)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    float t1;
    float t2;
    float b;
    float c;
    float3 dist = sub(camera_start, position);
    float a = dot(ray_arr[i], vector);
    a = dot(ray_arr[i], ray_arr[i]) - a * a;
    b = 2 * (dot(ray_arr[i], dist) - dot(ray_arr[i], vector) * \
    dot(dist, vector));
    c = dot(dist, vector);
    c = dot(dist, dist) - c * c - radius * radius;
    c = b * b - 4 * a * c;
    if (c >= 0)
    {
        c = sqrt(c);
        t1 = (-b + c) / (2 * a);
        t2 = (-b - c) / (2 * a);
        float result;
        result = 0;
        if ((t1 < t2 && t1 > 0) || (t2 < 0 && t1 >= 0))
            result = t1;
        if ((t2 < t1 && t2 > 0) || (t1 < 0 && t2 >= 0))
            result = t2;
        if (t2 == t1 && t2 >= 0)
            result = t2;
        if (result > 0 && result < depth_buf[i])
        {
            depth_buf[i] = result;
            index_buf[i] = index;
        }
    }
}