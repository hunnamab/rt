extern "C"{
    #include "rt_cuda.h"
	}
#include "rt.cuh"
void	print_gpu_info(void)
{
	int val;
	cudaDeviceProp *prop;

	prop = (cudaDeviceProp *)malloc(sizeof(cudaDeviceProp));
	cudaGetDeviceCount(&val);
	printf("device count %d\n", val);
	cudaGetDevice(&val);
	printf("current dev %d\n", val);
	cudaGetDeviceProperties(prop, 0);
	printf("device name %s\n", prop->name);
	printf("max threads per block %d\n", prop->maxThreadsPerBlock);
	printf("multiProcessorCount %d\n", prop->multiProcessorCount);
	printf("regsPerBlock %d\n", prop->regsPerBlock);
	printf("warp size %d\n", prop->warpSize);
	printf("canMapHostMemory %d\n", prop->canMapHostMemory);
	printf("maxGridSize[3] (%d,%d,%d)\n", prop->maxGridSize[0],prop->maxGridSize[1],prop->maxGridSize[2]);
}

void prepare(t_scene *scene, t_sphere *sphere)
{
    dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 256;
	blockSize = 256;
    intersect_ray_sphere_cl<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,sphere->center,sphere->radius,scene->device_data->depth_buf);
    cudaMemcpy(scene->depth_buf, scene->device_data->depth_buf, sizeof(float) * WID * HEI,cudaMemcpyDeviceToHost);
}

void prepare_t(t_scene *scene, t_triangle *t)
{
    dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 256;
	blockSize = 256;
	float3 *vertex;
	cudaMalloc(&vertex, sizeof(float3) * 3);
	cudaMemcpy(vertex, t->vertex,sizeof(float3) * 3, cudaMemcpyHostToDevice);
    intersect_ray_triangle_cl<<<gridSize,blockSize>>>(scene->device_data->ray_buf,scene->camera.position,scene->device_data->depth_buf,vertex, t->normal);
    cudaMemcpy(scene->depth_buf, scene->device_data->depth_buf, sizeof(float) * WID * HEI,cudaMemcpyDeviceToHost);
}