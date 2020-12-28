#include "rt.cuh"
extern "C"{
    #include "rt_cuda.h"
    }

__device__ void get_normal_sphere(t_object_device *obj, int *index_buf, float3 *normal_buf, float3 *intersection_buf, int index)
{
	t_sphere *s;
	int j;
	float l;

	j = index_buf[index];
	s = (t_sphere *)obj[j]->data;
	normal_buf[index] = sub(intersection_buf[index], s->center);
	l = length(normal_buf[index]);
	normal_buf[index] = div_by_scalar(normal_buf[index], l);
	if (dot(ray_buf[index], normal_buf[index]) > 0.0001)
		normal_buf[index] = scale(normal_buf[index], -1);
}
typedef void (*get_normal)(void);
__device__ get_normal p_get_normal_sphere = get_normal_sphere;

__global__ void	get_normal_buf_device(t_object_device *obj, int *index_buf, float3 *normal_buf, float3 *intersection_buf)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	int j = index_buf[i];
	if (obj[j])
}

__global__ void new_sphere_device(t_object_device *obj, get_normal *get_normal, float3 center, float radius, t_color color, \
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
	obj[index].get_normal = get_normal[0];
}
__global__ void test(t_object_device *objs, int index)
{
	t_sphere *s;

	s = reinterpret_cast<t_sphere *>(objs[index].data);
	printf("s->center %f\n", s->center);
	printf("s->radius %f\n", s->radius);
	printf("objs[%d].specular %f\n", objs[index].specular);
	objs[index].get_normal();
}
__host__ int    cuda_init(t_scene *scene)
{
	scene->device_data = (t_scene_device *)malloc(sizeof(t_scene_device));
	print_gpu_info();
	int err = 0;
	get_normal *host_get_normal_ptr;
	get_normal *dev_get_normal_ptr;
	host_get_normal_ptr = (get_normal *)malloc(sizeof(get_normal));
	cudaMalloc(&dev_get_normal_ptr, sizeof(get_normal));
	cudaMemcpyFromSymbol(&host_get_normal_ptr[0], p_get_normal_sphere, sizeof(get_normal));
	cudaMemcpy(dev_get_normal_ptr, host_get_normal_ptr, sizeof(get_normal), cudaMemcpyHostToDevice);
	int i = 0;
	//obj_init<<<1,1>>>(scene->device_data->objs, scene->obj_nmb);
	cudaMalloc(&scene->device_data->objs, sizeof(t_object_device) * scene->obj_nmb);
	while(i < scene->obj_nmb)
	{
		t_sphere *s;
		s = reinterpret_cast<t_sphere *>(scene->objs[i]->data);
		float *rotation;
		cudaMalloc(&rotation, sizeof(float) * 3);
		cudaMemcpy(rotation, scene->objs[i]->rotation, sizeof(float) * 3, cudaMemcpyHostToDevice);
		new_sphere_device<<<1,1>>>(scene->device_data->objs,dev_get_normal_ptr, s->center, s->radius, scene->objs[i]->color, rotation, scene->objs[i]->specular, i);
		cudaFree(&rotation);
		i++;
	}
	test<<<1,1>>>(scene->device_data->objs, 0);
	if((cudaMalloc(&scene->device_data->objs, sizeof(t_object_device) * scene->obj_nmb)) == CUDA_SUCCESS)
		printf("objs_allocated on device \n");
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
