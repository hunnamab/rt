/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:39 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 15:14:07 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_object	*new_cone(cl_float3 *pos_vec, float *ang_spec, t_color color, \
						float *rotation)
{
	t_cone		*new_cone;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	new_cone = malloc(sizeof(t_cone));
	new_cone->position = pos_vec[0];
	new_cone->vec = pos_vec[1];
	new_cone->angle = ang_spec[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_cone->vec, matrix, 1);
	matr_free(matrix, 4);
	new_object->specular = ang_spec[1];
	new_object->color = color;
	new_object->data = (void *)new_cone;
	new_object->tag = "cone";
	new_object->intersect = &intersect_ray_cone;
	new_object->get_normal = &get_cone_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void		get_cone_normal(t_scene *scene, int index, int obj_num)
{
	t_cone	*cone;
	float	m;
	cl_float3	*normal;
	cl_float3 buf;

	normal = &scene->normal_buf[index];
	cone = (t_cone *)scene->objs[obj_num]->data;
	buf = vector_sub(&scene->camera.position, &cone->position);
	m = vector_dot(&scene->ray_buf[index], &cone->vec) * \
					scene->depth_buf[index] + vector_dot(&buf, &cone->vec);
	buf = vector_scale(&cone->vec, m);
	*normal = vector_scale(&buf, (1 + cone->angle * cone->angle));
	buf = vector_sub(&scene->intersection_buf[index], &cone->position);
	*normal = vector_sub(&buf, normal);
	scene->normal_buf[index] = vector_div_by_scalar(&scene->normal_buf[index], \
								vector_length(&scene->normal_buf[index]));
	if (vector_dot(&scene->ray_buf[index], normal) > 0.0001)
		*normal = vector_scale(normal, -1);
}

/* float		intersect_ray_cone(t_scene *scene, int index, cl_float3 *start, cl_float3 *dir)
{
	float	a;
	float	b;
	float	c;
	cl_float3	dist;
	t_cone	*cone;

	cone = (t_cone *)scene->objs[index]->data;
	dist = vector_sub(start, &cone->position);
	a = vector_dot(dir, &cone->vec);
	a = vector_dot(dir, dir) - (1 + cone->angle * cone->angle) * a * a;
	b = 2 * (vector_dot(dir, &dist) - (1 + cone->angle * cone->angle) * \
		vector_dot(dir, &cone->vec) * vector_dot(&dist, &cone->vec));
	c = vector_dot(&dist, &cone->vec);
	c = vector_dot(&dist, &dist) - (1 + cone->angle * cone->angle) * c * c;
	c = b * b - 4 * a * c;
	c = DROUND(c);
	if (c >= 0)
	{
		c = sqrt(c);
		return (choose_t((-b + c) / (2 * a), (-b - c) / (2 * a)));
	}
	return (0);
} */

void		intersect_ray_cone(t_scene *scene, int index)
{
	size_t global = WID * HEI;

	cl_mem position;
	cl_mem vector;
	cl_mem angle;
	cl_mem ind;

	size_t local;
	t_cone *c = (t_cone *)scene->objs[index]->data;

	position = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	vector = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	angle = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float), NULL, NULL);
	ind = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(int), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, position, CL_FALSE, 0, sizeof(cl_float3), &c->position, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, vector, CL_FALSE, 0, sizeof(cl_float3), &c->vec, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, angle, CL_FALSE, 0, sizeof(float), &c->angle, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, ind, CL_FALSE, 0, sizeof(int), &index, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[2], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
    clSetKernelArg(scene->cl_data.kernels[2], 2, sizeof(cl_mem), &position);
	clSetKernelArg(scene->cl_data.kernels[2], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 4, sizeof(cl_mem), &vector);
	clSetKernelArg(scene->cl_data.kernels[2], 5, sizeof(cl_mem), &angle);
	clSetKernelArg(scene->cl_data.kernels[2], 6, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 7, sizeof(cl_mem), &ind);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[2], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[2], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL);
	clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.index_buf, CL_TRUE, 0, sizeof(int) * global, scene->index_buf, 0, NULL, NULL);
}
