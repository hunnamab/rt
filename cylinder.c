/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:11:46 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 15:42:27 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

t_object	*new_cylinder(cl_float3 *pos_vec, float *rad_spec, t_color color, \
							float *rotation)
{
	t_cylinder	*new_cylinder;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_cylinder = protected_malloc(sizeof(t_cylinder), 1);
	new_cylinder->position = pos_vec[0];
	new_cylinder->radius = rad_spec[0];
	new_cylinder->vec = pos_vec[1];
	new_object->specular = rad_spec[1];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_cylinder->vec, matrix, 1);
	matr_free(matrix, 4);
	new_object->color = color;
	new_object->data = (void *)new_cylinder;
	new_object->tag = "cylinder";
	new_object->type = CYLINDER;
	new_object->intersect = &intersect_ray_cylinder;
	new_object->get_normal = &get_cylinder_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void		get_cylinder_normal(t_scene *scene, int index, int obj_num)
{
	t_cylinder	*cylinder;
	cl_float3		*normal;
	float		m;
	cl_float3		p;
	cl_float3		buf[2];

	normal = &scene->normal_buf[index];
	cylinder = (t_cylinder *)scene->objs[obj_num]->data;
	buf[0] = vector_sub(&scene->camera.position, &cylinder->position);
	m = vector_dot(&scene->ray_buf[index], &cylinder->vec) * \
		scene->depth_buf[index] + vector_dot(&buf[0], &cylinder->vec);
	buf[0] = vector_scale(&scene->ray_buf[index], scene->depth_buf[index]);
	p = vector_add(&scene->camera.position, &buf[0]);
	buf[0] = vector_sub(&p, &cylinder->position);
	buf[1] = vector_scale(&cylinder->vec, m);
	*normal = vector_sub(&buf[0], &buf[1]);
	scene->normal_buf[index] = vector_div_by_scalar(&scene->normal_buf[index], \
								vector_length(&scene->normal_buf[index]));
	if (vector_dot(&scene->ray_buf[index], normal) > 0.0001)
		*normal = vector_scale(normal, -1);
}

/* float		intersect_ray_cylinder(t_scene *scene, int index, cl_float3 *start, cl_float3 *dir)
{
	float		a;
	float		b;
	float		c;
	cl_float3	dist;
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)scene->objs[index]->data;
	dist = vector_sub(start, &cylinder->position);
	a = vector_dot(dir, &cylinder->vec);
	a = vector_dot(dir, dir) - a * a;
	b = 2 * (vector_dot(dir, &dist) - \
				vector_dot(dir, &cylinder->vec) * \
				vector_dot(&dist, &cylinder->vec));
	c = vector_dot(&dist, &cylinder->vec);
	c = vector_dot(&dist, &dist) - c * c - cylinder->radius * cylinder->radius;
	c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		return (choose_t((-b + c) / (2 * a), (-b - c) / (2 * a)));
	}
	return (0);
} */

void		intersect_ray_cylinder(t_scene *scene, int index)
{
	size_t global = WID * HEI;

	cl_mem position;

	size_t local;
	t_cylinder *cyl = (t_cylinder *)scene->objs[index]->data;

	position = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, position, CL_FALSE, 0, sizeof(cl_float3), &cyl->position, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[3], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
    clSetKernelArg(scene->cl_data.kernels[3], 2, sizeof(cl_mem), &position);
	clSetKernelArg(scene->cl_data.kernels[3], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 4, sizeof(cl_float3), (void *)&cyl->vec);
	clSetKernelArg(scene->cl_data.kernels[3], 5, sizeof(cl_float), (void *)&cyl->radius);
	clSetKernelArg(scene->cl_data.kernels[3], 6, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 7, sizeof(cl_int), (void*)&index);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[3], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[3], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
}
