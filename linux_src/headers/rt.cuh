#ifndef RT_CUH
# define RT_CUH
# include <cuda_runtime_api.h>
# include <vector_types.h>
# include <cuda.h>
# include <SDL2/SDL.h> //for linux
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdint.h>
# include <unistd.h>
#include "device_launch_parameters.h"

__global__ void kernel_getray(float3 *viewport, float3 *camera, float3 *ray_buf);

#endif