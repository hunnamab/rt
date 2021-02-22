/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 21:42:26 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/22 16:37:27 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			triangle_init(t_object *new_object, float specular, \
								float *rotation, t_color color)
{
	new_object->normal_text = NULL;
	new_object->specular = specular;
	new_object->reflection = rotation[3];
	new_object->refraction = rotation[4];
	new_object->transparency = rotation[5];
	new_object->t_scale = rotation[6];
	new_object->text = NULL;
	new_object->surface_id = rotation[7];
	new_object->color = color;
	new_object->tag = "triangle";
	new_object->type = TRIANGLE;
	new_object->cs_nmb = 0;
	new_object->cutting_surfaces = NULL;
	new_object->intersect = &intersect_ray_triangle;
	new_object->get_normal = &get_triangle_normal;
	new_object->clear_obj = &clear_triangle;
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
}

t_object		*new_triangle(cl_float3 *vertex, float specular, \
										t_color color, float *rotation)
{
	t_triangle	*new_triangle;
	t_object	*new_object;
	cl_float3	edge1;
	cl_float3	edge2;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_triangle = protected_malloc(sizeof(t_triangle), 1);
	init_norme_triangle(rotation, vertex, \
			new_object->rotation, new_triangle->vertex);
	matrix = get_rotation_matrix(new_object->rotation);
	transform(new_triangle->vertex, matrix, 3);
	matr_free(matrix, 4);
	edge1 = vector_sub(&new_triangle->vertex[0], &new_triangle->vertex[1]);
	edge2 = vector_sub(&new_triangle->vertex[1], &new_triangle->vertex[2]);
	new_object->data = (void *)new_triangle;
	new_triangle->normal = vector_cross(&edge1, &edge2);
	normalize_vector(&new_triangle->normal);
	triangle_init(new_object, specular, rotation, color);
	new_object->basis = get_basis_tr(new_object->basis, new_triangle->vertex);
	return (new_object);
}

void			get_triangle_normal(t_scene *scene, int index, int obj_num)
{
	t_triangle *t;

	t = (t_triangle *)scene->objs[obj_num]->data;
	copy_point(&scene->normal_buf[index], &t->normal);
	if (vector_dot(&scene->ray_buf[index], \
	&scene->normal_buf[index]) > 0.0001)
		scene->normal_buf[index] = vector_scale(&scene->normal_buf[index], -1);
}

void			set_arg(t_scene *scene, \
								int index, cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[4], 0, \
	sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[4], 1, \
	sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[4], 2, \
	sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[4], 3, \
	sizeof(t_triangle), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[4], 4, \
	sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[4], 5, \
	sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[4], 6, \
	sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[4], 7, \
	sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[4], 8, \
	sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[4], 9, \
	sizeof(cl_mem), &scene->cl_data.scene.material_buf);
}

void			intersect_ray_triangle(t_scene *scene, \
								int index, int is_refractive)
{
	size_t global;
	size_t local;
	cl_mem cs;

	global = WID * HEI;
	is_refractive = 0;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, \
		sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb, \
		scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	set_arg(scene, index, cs);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[4], \
	scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
	sizeof(local), &local, NULL);
	ft_printf("local == max work group size == %ld\n", local);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
	scene->cl_data.kernels[4], 1, NULL, &global, &local, 0, NULL, NULL);
}
