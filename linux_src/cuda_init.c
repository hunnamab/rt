#include "rt_cuda.h"

__host__ int    cuda_init(t_scene *scene)
{
	if((cudaMalloc(scene->device_data.ray_buf, sizeof(float3) * WID * HEI)))
		printf("device ray_buf allocated\n");
	if((cudaMalloc(scene->device_data.intersection_buf, sizeof(float3) * WID * HEI)))
		printf("device intersection_buf allocated\n");
	if((cudaMalloc(scene->device_data.viewport, sizeof(float3) * WID * HEI)))
		printf("device viewport allocated\n");
	if((cudaMalloc(scene->device_data.index_buf, sizeof(int) * WID * HEI)))
		printf("device index_buf allocated\n");
	if((cudaMalloc(scene->device_data.depth_buf, sizeof(float) * WID * HEI)))
		printf("device depth_buf allocated\n");
	if((cudaMalloc(scene->device_data.normal_buf, sizeof(float3) * WID * HEI)))
		printf("device normal_buf allocated\n");
	if((cudaMalloc(scene->device_data.camera, sizeof(float3))))
		printf("device camera allocated\n");
	return (0);
}
