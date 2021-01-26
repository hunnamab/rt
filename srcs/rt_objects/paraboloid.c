#include "rt.h"

t_object    *new_paraboloid(cl_float3 center, float k, t_color color, cl_float3 rotation, float specular)
{
    t_paraboloid *parab;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	parab = malloc(sizeof(t_paraboloid));
	parab->center = center; 
	parab->k = k;
	new_object->rotation[0] = rotation.x;
	new_object->rotation[1] = rotation.y;
	new_object->rotation[2] = rotation.z;
	new_object->specular = specular;
	new_object->color = color;
	new_object->text = NULL;
	new_object->data = (void *)parab;
	new_object->type = PARABOLOID;
	new_object->intersect = &intersect_ray_paraboloid;
	//new_object->get_normal = &get_paraboloid_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void        intersect_ray_paraboloid(t_scene *scene, int index)
{
    size_t global = WID * HEI;
	size_t local;

    clSetKernelArg(scene->cl_data.kernels[12], 0, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[12], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[12], 2, sizeof(t_paraboloid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[12], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[12], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[12], 5, sizeof(cl_int), (void*)&index);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[12], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("paraboloid local == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[12], 1, NULL, &global, &local, 0, NULL, NULL);
}