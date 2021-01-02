#include "rt.cuh"
extern "C"{
    #include "rt_cuda.h"
    }

__host__ int    cuda_init(t_scene *scene)
{
	scene->device_data = (t_scene_device *)malloc(sizeof(t_scene_device));
	print_gpu_info();
	int err = 0;
	
	if((cudaMalloc(&scene->device_data->ray_buf, sizeof(float3) * WID * HEI)) == CUDA_SUCCESS)
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
	if((cudaMalloc(&scene->device_data->frame_buf, sizeof(t_color) * WID * HEI) == CUDA_SUCCESS))
		printf("device normal_buf allocated\n");
	else
		printf("device normal_buf alloc error\n");
	if((cudaMalloc(&scene->device_data->camera, sizeof(float3))) == CUDA_SUCCESS)
		printf("device camera allocated\n");
	if((cudaMalloc(&scene->device_data->material_buf, sizeof(t_material) * WID * HEI) == CUDA_SUCCESS))
		printf("device material_buf allocated\n");
	else
		printf("device material_buf alloc error\n");
	device_objects_init(scene);
	device_light_init(scene);
	return (0);
}
