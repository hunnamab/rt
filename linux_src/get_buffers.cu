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

 __global__ void kernel_get_intersection_point(float3 *intersection_buf, float3 *ray_buf, float *depth_buf, float3 camera, int *index_buf)
 {
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (index_buf[i] != -1)
    {
        intersection_buf[i] = scale(ray_buf[i], depth_buf[i]);
            intersection_buf[i] = add(\
            intersection_buf[i], camera);
    }
    else
    {
        intersection_buf[i].x = 0;
        intersection_buf[i].y = 0;
        intersection_buf[i].z = 0;
    }
}