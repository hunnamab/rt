#include "rt.h"

t_object    *new_torus(cl_float3 center, float radius1, float radius2, t_color color, cl_float3 rotation, float specular)
{
    t_torus	*torus;
	t_object		*new_object;
	float			**matrix;

	new_object = malloc(sizeof(t_object));
	torus = malloc(sizeof(t_torus));
	torus->center = center; 
	torus->radius1 = radius1;
	torus->radius2 = radius2;
	new_object->rotation[0] = rotation.x;
	new_object->rotation[1] = rotation.y;
	new_object->rotation[2] = rotation.z;
	new_object->specular = specular;
	new_object->reflection = 0.0;
	new_object->color = color;
	new_object->text = NULL;
	new_object->data = (void *)torus;
	new_object->type = TORUS;
	new_object->cs_nmb = 0;
	new_object->intersect = &intersect_ray_torus;
	//new_object->get_normal = &get_torus_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void        intersect_ray_torus(t_scene *scene, int index)
{
    size_t global = WID * HEI;
	size_t local;
	cl_int err = 0;
    clSetKernelArg(scene->cl_data.kernels[13], 0, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[13], 2, sizeof(t_torus), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[13], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 5, sizeof(cl_int), (void*)&index);
    clGetKernelWorkGroupInfo(scene->cl_data.kernels[13], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("torus local == %ld\n", local);
    err = clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[13], 1, NULL, &global, &local, 0, NULL, NULL);
	printf("%d\n", err);
}
