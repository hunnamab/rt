#include "rt.cuh"

__global__ void kernel_getray(float3 *viewport, float3 *camera, float3 *ray_buf)
{
    int i = blockDim.x * blockIdx.x * threadIdx.x;

    ray_buf[i].x = viewport[i].x - camera[0].x;
    ray_buf[i].y = viewport[i].y - camera[0].y;
    ray_buf[i].z = viewport[i].z - camera[0].z;
}