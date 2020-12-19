#ifndef RT_CUH
# define RT_CUH
# include <cuda_runtime_api.h>
# include <vector_types.h>
# include <cuda.h>
# include <stdio.h>
# include <math.h>
#include "device_launch_parameters.h"
# include "types_cuda.h"
#if defined(__CUDA_ARCH__) && (__CUDA_ARCH__ < 200)
    #define printf(f, ...) ((void)(f, __VA_ARGS__),0)
#endif
__global__ void kernel_getray(float3 *viewport, float3 camera, float3 *ray_buf);
__host__ void	print_gpu_info(void);
__global__ void helloCUDA(float f);
#endif