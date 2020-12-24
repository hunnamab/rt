#ifndef RT_CUH
# define RT_CUH
# include <cuda_runtime_api.h>
# include <vector_types.h>
# include <cuda.h>
# include <stdio.h>
# include <math.h>
#include "device_launch_parameters.h"
# include "types_cuda.h"
# define WID 1280
# define HEI 720
#if defined(__CUDA_ARCH__) && (__CUDA_ARCH__ < 200)
    #define printf(f, ...) ((void)(f, __VA_ARGS__),0)
#endif
__global__ void kernel_getray(float3 *viewport, float3 camera, float3 *ray_buf);
__global__ void helloCUDA(float f);
__global__ void intersect_ray_sphere_c(float3 *ray_arr, float3 camera_start, float3 s_center, \
     float s_radius, float *depth_buf, int *index_buf, int index);
__global__ void intersect_ray_triangle_c(float3 *ray_arr, float3 camera_start, float *depth_buf, \
     float3 *vertex, int *index_buf, int index);
__global__ void intersect_ray_cone_c(float3 *ray_arr, float3 camera_start, float3 position, \
    float3 vector, float angle, float *depth_buf, int *index_buf, int index);
__global__ void intersect_ray_plane_c(float3 *ray_arr, float3 camera_start, \
     float *depth_buf, float3 normal, float d, int *index_buf, int index);
__global__ void intersect_ray_cylinder_c(float3 *ray_arr, float3 camera_start, \
    float3 position, float *depth_buf, float3 vector, \
    float radius, int *index_buf, int index);

#endif