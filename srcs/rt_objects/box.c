#include "rt.h"

t_object    *new_box(cl_float3 a, cl_float3 b, t_color color, cl_float3 rotation, float specular)
{
    t_box *box;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	box = malloc(sizeof(t_box));
	box->a = a;
    box->b = b;
	new_object->rotation[0] = rotation.x;
	new_object->rotation[1] = rotation.y;
	new_object->rotation[2] = rotation.z;
	new_object->specular = specular;
	new_object->color = color;
	new_object->text = NULL;
	new_object->data = (void *)box;
	new_object->type = BOX;
	new_object->intersect = &intersect_ray_box;
	//new_object->get_normal = &get_box_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void        intersect_ray_box(t_scene *scene, int index)
{
    size_t global = WID * HEI;
	size_t local;

	clSetKernelArg(scene->cl_data.kernels[11], 0, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[11], 2, sizeof(t_box), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[11], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 5, sizeof(cl_int), (void*)&index);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[11], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[11], 1, NULL, &global, &local, 0, NULL, NULL);
}