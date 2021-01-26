/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:39 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/19 19:57:26 by ldeirdre         ###   ########.fr       */
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
	new_object->reflection = 0.0;
	new_object->color = color;
	new_object->text = NULL;
	new_object->data = (void *)new_cone;
	new_object->tag = "cone";
	new_object->type = CONE;
	new_object->intersect = &intersect_ray_cone;
	new_object->get_normal = &get_cone_normal;
	new_object->clear_obj = &clear_default;
	new_object->basis = get_default(new_object->basis);
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
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
	size_t local;

	clSetKernelArg(scene->cl_data.kernels[2], 0, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 1,sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 3, sizeof(t_cone), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[2], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 5, sizeof(cl_int), (void*)&index);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[2], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[2], 1, NULL, &global, &local, 0, NULL, NULL);
}

void	one_argument_cone(char **description, t_scene *scene, int *snmi)
{
	t_object	*cone;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rotation[3];
	float		ang_spec[2];
	
	pos_vec_buf[0] = get_points(description[1]);
	pos_vec_buf[1] = get_points(description[2]);
	ang_spec[0] = ftoi(get_coordinates(description[3]));
	pos_vec_buf[2] = get_points(description[4]);
	rotation[0] = pos_vec_buf[2].x;
	rotation[1] = pos_vec_buf[2].y;
	rotation[2] = pos_vec_buf[2].z;
	color = get_color(description[5]);
	ang_spec[1] = ftoi(get_coordinates(description[6]));
	cone = new_cone(pos_vec_buf, ang_spec, color, rotation);
	cone->text = tex_new_bmp(get_file(description[7]));
	scene->objs[snmi[1]] = cone;
	snmi[1]++;
}

t_object 	*multiple_cones(char **description, t_scene *scene, int *snmi, int i)
{
	t_object	*cone;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rotation[3];
	float		ang_spec[2];

	pos_vec_buf[0] = get_points(description[i + 1]);
	pos_vec_buf[1] = get_points(description[i + 2]);
	ang_spec[0] = ftoi(get_coordinates(description[i + 3]));
	pos_vec_buf[2] = get_points(description[i + 4]);
	rotation[0] = pos_vec_buf[2].x;
	rotation[1] = pos_vec_buf[2].y;
	rotation[2] = pos_vec_buf[2].z;
	color = get_color(description[i + 5]);
	ang_spec[1] = ftoi(get_coordinates(description[i + 6]));
	cone = new_cone(pos_vec_buf, ang_spec, color, rotation);
	return (cone);
}
