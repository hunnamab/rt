#include "rt.cuh"

__global__ void helloCUDA(float f)
{
    printf("Hello thread %d, f=%f\n", threadIdx.x, f);
}

__global__ void kernel_getray(float3 *viewport, float3 camera, float3 *ray_buf)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    
    ray_buf[i].x = viewport[i].x - camera.x;
    ray_buf[i].y = viewport[i].y - camera.y;
    ray_buf[i].z = viewport[i].z - camera.z; 
}