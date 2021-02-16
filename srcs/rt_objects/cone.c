/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:39 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/16 18:07:37 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_object		*new_cone(cl_float3 *pos_vec, float *ang_spec, t_color color, \
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
	new_object->reflection = ang_spec[2];
	new_object->refraction = ang_spec[3];
	new_object->transparency = ang_spec[4];
	new_object->t_scale = ang_spec[5];
	new_object->position = pos_vec[0];
	new_object->cs_nmb = 0;
	new_object->cutting_surfaces = NULL;
	new_object->surface_id = ang_spec[6];
	new_object->color = color;
	new_object->text = NULL;
	new_object->normal_text = NULL;
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

void			get_cone_normal(t_scene *scene, int index, int obj_num)
{
	t_cone		*cone;
	float		m;
	cl_float3	*normal;
	cl_float3	buf;

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

void			intersect_ray_cone(t_scene *scene, int index, int is_refractive)
{
	size_t		global;
	size_t		local;
	cl_mem		cs;

	global = WID * HEI;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
			sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb,
						scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	clSetKernelArg(scene->cl_data.kernels[2], 0, sizeof(cl_mem),
									&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 1, sizeof(cl_mem),
									&scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 2, sizeof(cl_mem),
									&scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 3, sizeof(t_cone),
									scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[2], 4, sizeof(cl_mem),
									&scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 5, sizeof(cl_int),
									(void*)&index);
	clSetKernelArg(scene->cl_data.kernels[2], 6, sizeof(cl_int),
									(void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[2], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[2], 8, sizeof(cl_int),
									(void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[2], 9, sizeof(cl_mem),
									&scene->cl_data.scene.material_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[2],
	scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE,
					sizeof(local), &local, NULL);
	ft_printf("local == max work group size == %ld\n", local);
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[2],
									1, NULL, &global, &local, 0, NULL, NULL);
}

void			one_argument_cone(char **description, t_scene *scene, int *snmi)
{
	t_object	*cone;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rotation[3];
	float		ang_spec[7];

	pos_vec_buf[0] = get_points(description[1]);
	pos_vec_buf[1] = get_points(description[2]);
	ang_spec[0] = ftoi(get_coordinates(description[3]));
	pos_vec_buf[2] = get_points(description[4]);
	rotation[0] = pos_vec_buf[2].x;
	rotation[1] = pos_vec_buf[2].y;
	rotation[2] = pos_vec_buf[2].z;
	color = get_color(description[5]);
	ang_spec[1] = ftoi(get_coordinates(description[6]));
	ang_spec[2] = ftoi(get_coordinates(description[7]));
	ang_spec[3] = ftoi(get_coordinates(description[8]));
	ang_spec[4] = ftoi(get_coordinates(description[9]));
	ang_spec[5] = ftoi(get_coordinates(description[10]));
	ang_spec[6] = ftoi(get_coordinates(description[11]));
	cone = new_cone(pos_vec_buf, ang_spec, color, rotation);
	cone->text = tex_new_bmp(get_file(description[12]));
	cone->normal_text = tex_new_bmp(get_file(description[13]));
	scene->objs[snmi[1]] = cone;
	snmi[1]++;
}

t_object		*multiple_cones(char **description, int i)
{
	t_object	*cone;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rotation[3];
	float		ang_spec[7];

	pos_vec_buf[0] = get_points(description[i + 1]);
	pos_vec_buf[1] = get_points(description[i + 2]);
	ang_spec[0] = ftoi(get_coordinates(description[i + 3]));
	pos_vec_buf[2] = get_points(description[i + 4]);
	rotation[0] = pos_vec_buf[2].x;
	rotation[1] = pos_vec_buf[2].y;
	rotation[2] = pos_vec_buf[2].z;
	color = get_color(description[i + 5]);
	ang_spec[1] = ftoi(get_coordinates(description[i + 6]));
	ang_spec[2] = ftoi(get_coordinates(description[i + 7]));
	ang_spec[3] = ftoi(get_coordinates(description[i + 8]));
	ang_spec[4] = ftoi(get_coordinates(description[i + 9]));
	ang_spec[5] = ftoi(get_coordinates(description[i + 10]));
	ang_spec[6] = ftoi(get_coordinates(description[i + 11]));
	cone = new_cone(pos_vec_buf, ang_spec, color, rotation);
	return (cone);
}
