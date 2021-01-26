#include "rt.h"

t_object    *new_ellipsoid(cl_float3 center, cl_float3 abc, t_color color, cl_float3 rotation, float specular)
{
    t_ellipsoid *el;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	el = malloc(sizeof(t_ellipsoid));
	el->center = center; 
	el->abc = abc;
	new_object->rotation[0] = rotation.x;
	new_object->rotation[1] = rotation.y;
	new_object->rotation[2] = rotation.z;
	new_object->specular = specular;
	new_object->color = color;
	new_object->text = NULL;
	new_object->data = (void *)el;
	new_object->type = ELLIPSOID;
	new_object->intersect = &intersect_ray_ellipsoid;
	//new_object->get_normal = &get_ellipsoid_normal;
	new_object->clear_obj = &clear_default;
    printf("abc == (%f,%f,%f)\n", el->abc.x,el->abc.y,el->abc.z);
	return (new_object);
}

void        intersect_ray_ellipsoid(t_scene *scene, int index)
{
    size_t global = WID * HEI;
	size_t local;

	clSetKernelArg(scene->cl_data.kernels[8], 0, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[8], 2, sizeof(t_ellipsoid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[8], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 5, sizeof(cl_int), (void*)&index);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[8], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("ellipsoid local == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[8], 1, NULL, &global, &local, 0, NULL, NULL);
}