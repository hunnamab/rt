#include "rt_cuda.h"


__host__ int    cuda_init(t_scene *scene)
{
	scene->device_data = malloc(sizeof(t_scene_device));
	print_gpu_info();
	int err = 0;
	if((cudaMalloc((void **)&scene->device_data->ray_buf, sizeof(float3) * WID * HEI)) == CUDA_SUCCESS)
		printf("device ray_buf allocated\n");
	else
		printf("device ray_buf alloc error %d\n", err);
	if((cudaMalloc(&scene->device_data->intersection_buf, sizeof(float3) * WID * HEI) == CUDA_SUCCESS))
		printf("device intersection_buf allocated\n");
	else
		printf("device intersection_buf alloc error\n");
 	if((cudaMalloc(&scene->device_data->viewport, sizeof(float3) * WID * HEI) == CUDA_SUCCESS))
		printf("device viewport allocated\n");
	else
		printf("device viewport alloc error\n");
 	if((cudaMalloc(&scene->device_data->index_buf, sizeof(int) * WID * HEI) == CUDA_SUCCESS))
		printf("device index_buf allocated\n");
	else
		printf("device index_buf alloc error\n");
	if((cudaMalloc(&scene->device_data->depth_buf, sizeof(float) * WID * HEI) == CUDA_SUCCESS))
		printf("device depth_buf allocated\n");
	else
		printf("device depth_buf alloc error\n");
	if((cudaMalloc(&scene->device_data->normal_buf, sizeof(float3) * WID * HEI) == CUDA_SUCCESS))
		printf("device normal_buf allocated\n");
	else
		printf("device normal_buf alloc error\n");
	return (0);
}
