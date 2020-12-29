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

__device__ float	length(float3 vector)
{
	float	res;

	res = dot(vector, vector);
	return (sqrt(res));
}

__device__ float3	div_by_scalar(float3 vector, float scalar)
{
	float3 result;

	result.x = vector.x / scalar;
	result.y = vector.y / scalar;
	result.z = vector.z / scalar;
	return (result);
}

 __device__ float3		scale(float3 v, float c)
{
	float3 res;

	res.x = v.x * c;
	res.y = v.y * c;
	res.z = v.z * c;
	return (res);
}

__device__ float3		add(const float3 v1, const float3 v2)
{
	float3 res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

__device__ float3	cross(float3 v1, float3 v2)
{
	float3 result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return (result);
}