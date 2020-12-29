#include "rt.cuh"
extern "C"{
    #include "rt_cuda.h"
    }

__device__ void get_normal_sphere(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index)
{
	float l;
	int j;

	j = index_buf[index];
	normal_buf[index] = sub(intersection_buf[index], obj[j].sphere.center);
	l = length(normal_buf[index]);
	normal_buf[index] = div_by_scalar(normal_buf[index], l);
	if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
		normal_buf[index] = scale(normal_buf[index], -1);
}

//__global__ void	get_normal_buf_device(t_object_device *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf)
__global__ void	get_normal_buf_device(t_object_d *obj, float3 *ray_buf, int *index_buf, float3 *normal_buf, float3 *intersection_buf)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	int j = index_buf[i];
/* 	int j = index_buf[i];
	
 	if(/* obj[j].type == SPHERE &&  j != -1)
 	{
		t_sphere *s;
		s = reinterpret_cast<t_sphere *>(obj[j].data);
		get_normal_sphere(s, ray_buf, index_buf, normal_buf,intersection_buf, i);
	} */
	if(/* obj[j].type == SPHERE &&*/  j != -1)
	{
		get_normal_sphere(obj,ray_buf, index_buf, normal_buf,intersection_buf, i);
	}
}

__global__ void new_sphere_device(t_object_device *obj, float3 center, float radius, t_color color, \
	float *rotation, float specular, int index)
{
	t_sphere *s;
	s = (t_sphere *)malloc(sizeof(t_sphere));
	s->center = center;
	s->radius = radius;
	obj[index].data = s;
	obj[index].color = color;
	obj[index].specular = specular;
	obj[index].rotation = (float *)malloc(sizeof(float) * 3);
	obj[index].rotation[0] = rotation[0];
	obj[index].rotation[1] = rotation[1];
	obj[index].rotation[2] = rotation[2];
}

__host__ int    cuda_init(t_scene *scene)
{
	scene->device_data = (t_scene_device *)malloc(sizeof(t_scene_device));
	print_gpu_info();
	int err = 0;
	
	/* if((cudaMalloc(&scene->device_data->objs, sizeof(t_object_device) * scene->obj_nmb)) == CUDA_SUCCESS)
		printf("objs_allocated on device \n"); */
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
	/* int i = 0;
	while(i < scene->obj_nmb)
		{
			t_sphere *s;
			s = reinterpret_cast<t_sphere *>(scene->objs[i]->data);
			float *rotation;
			cudaMalloc(&rotation, sizeof(float) * 3);
			cudaMemcpy(rotation, scene->objs[i]->rotation, sizeof(float) * 3, cudaMemcpyHostToDevice);
			new_sphere_device<<<1,1>>>(scene->device_data->objs, s->center, s->radius, scene->objs[i]->color, rotation, scene->objs[i]->specular, i);
			cudaFree(&rotation);
			i++;
		} */
	return (0);
}
