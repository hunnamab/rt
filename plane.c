/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:22:24 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:59:39 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_object	*new_plane(cl_float3 *poi_nor, float specular, t_color color, \
						float *rotation)
{
	t_plane		*new_plane;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_plane = protected_malloc(sizeof(t_plane), 1);
	new_plane->normal = poi_nor[1];
	normalize_vector(&new_plane->normal);
	new_plane->point = poi_nor[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_plane->normal, matrix, 1);
	matr_free(matrix, 4);
	new_object->specular = specular;
	new_object->color = color;
	new_object->data = (void *)new_plane;
	new_object->tag = "plane";
	new_object->intersect = &intersect_ray_plane;
	new_object->get_normal = &get_plane_normal;
	new_plane->d = -new_plane->normal.x * new_plane->point.x - new_plane->\
	normal.y * new_plane->point.y - new_plane->normal.z * new_plane->point.z;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void		get_plane_normal(t_scene *scene, int index, int obj_num)
{
	t_plane *p;

	p = (t_plane *)scene->objs[obj_num]->data;
	copy_point(&scene->normal_buf[index], &p->normal);
	if (vector_dot(&scene->ray_buf[index], \
		&scene->normal_buf[index]) > 0.0001)
		scene->normal_buf[index] = vector_scale(&scene->normal_buf[index], -1);
}

/* float		intersect_ray_plane(t_scene *scene, int index, cl_float3 *start, cl_float3 *dir)
{
	float	k1;
	float	k2;
	t_plane	*plane;

	plane = (t_plane *)scene->objs[index]->data;
	if ((vector_dot(dir, &plane->normal)) == 0)
		return (0);
	k1 = vector_dot(start, &plane->normal) + plane->d;
	k2 = vector_dot(dir, &plane->normal);
	if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
		return (0);
	return (-k1 / k2);
} */

void		intersect_ray_plane(t_scene *scene, int index)
{
	size_t global = WID * HEI;

	cl_mem normal;
	cl_mem d;
	cl_mem ind;

	size_t local;
	t_plane *p = (t_plane *)scene->objs[index]->data;

	normal = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	d = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float), NULL, NULL);
	ind = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(int), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, normal, CL_FALSE, 0, sizeof(cl_float3), &p->normal, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, d, CL_FALSE, 0, sizeof(float), &p->d, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, ind, CL_FALSE, 0, sizeof(int), &index, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[5], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
	clSetKernelArg(scene->cl_data.kernels[5], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 3, sizeof(cl_mem), &normal);
	clSetKernelArg(scene->cl_data.kernels[5], 4, sizeof(cl_mem), &d);
	clSetKernelArg(scene->cl_data.kernels[5], 5, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 6, sizeof(cl_mem), &ind);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[5], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[5], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
}
