#include "rt.h"

t_object    *new_hyperboloid(cl_float3 center, float a, float b, float c, t_color color)
{
    t_hyperboloid *hyper;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	hyper = malloc(sizeof(t_hyperboloid));
	hyper->center = center; 
	hyper->a = a;  
	hyper->b = b;
	hyper->c = c;
	new_object->rotation[0] = 0;//buf[2].x;
	new_object->rotation[1] = 0;//buf[2].y;
	new_object->rotation[2] = 0;//buf[2].z;
	new_object->specular = 0;//specular[1];
	new_object->reflection = 0;//specular[2];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->data = (void *)hyper;
	new_object->type = HYPERBOLOID;
	new_object->intersect = &intersect_ray_hyperboloid;
	//new_object->get_normal = &get_hyperboloid_normal;
	new_object->clear_obj = &clear_default;
    //printf("abc == (%f,%f,%f)\n", hyper->abc.x,hyper->abc.y,hyper->abc.z);
	return (new_object);
}

void        intersect_ray_hyperboloid(t_scene *scene, int index)
{
    size_t global = WID * HEI;
	size_t local;
	cl_mem cs;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb, scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
    printf("\n\nsizeof hyperbaloid HOST %lu\n\n", sizeof(t_hyperboloid));
	clSetKernelArg(scene->cl_data.kernels[14], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[14], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[14], 2, sizeof(t_hyperboloid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[14], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[14], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[14], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[14], 6, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[14], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[14], 8, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[14], 9, sizeof(cl_mem), &scene->cl_data.scene.material_buf);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[14], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("hyperboloid local == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[14], 1, NULL, &global, &local, 0, NULL, NULL);
}
