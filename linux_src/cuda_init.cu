#include "rt.cuh"
extern "C"{
    #include "rt_cuda.h"
    }

__device__ void lol(void)
{
	printf("kek is real\n");
}
typedef void (*lol_p)(void);
__device__ lol_p pfunc1 = lol;

__global__ void  obj_init(t_object_device *objs, lol_p *kek)
{
	objs[0].data = malloc(sizeof(t_sphere));
	/* if((malloc(&objs[0].data, sizeof(t_sphere))) == CUDA_SUCCESS) */
	if(objs[0].data != NULL)	
		printf("objs->data allocated on device \n");
	objs->lol_p = kek[0];
	objs->lol_p();
}
__host__ int    cuda_init(t_scene *scene)
{
	scene->device_data = (t_scene_device *)malloc(sizeof(t_scene_device));
	print_gpu_info();
	int err = 0;
	lol_p *host_lol_ptr;
	lol_p *dev_lol_ptr;
	host_lol_ptr = (lol_p *)malloc(sizeof(lol_p));
	cudaMalloc(&dev_lol_ptr, sizeof(lol_p));
	cudaMemcpyFromSymbol(&host_lol_ptr[0], pfunc1, sizeof(lol_p));
	cudaMemcpy(dev_lol_ptr, host_lol_ptr, sizeof(lol_p), cudaMemcpyHostToDevice);
	if((cudaMalloc(&scene->device_data->objs, sizeof(t_object_device) * scene->obj_nmb)) == CUDA_SUCCESS)
		printf("objs_allocated on device \n");
	obj_init<<<1,1>>>(scene->device_data->objs, dev_lol_ptr);
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
	if((cudaMalloc(&scene->device_data->camera, sizeof(float3))) == CUDA_SUCCESS)
		printf("device camera allocated\n");
	return (0);
}
