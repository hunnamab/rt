/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 02:00:56 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 02:10:24 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		plane_init(t_object *new_object, cl_float3 *poi_nor, \
									float *specular, t_color color)
{
	new_object->rotation[0] = poi_nor[2].x;
	new_object->rotation[1] = poi_nor[2].y;
	new_object->rotation[2] = poi_nor[2].z;
	new_object->position = poi_nor[0];
	new_object->specular = specular[0];
	new_object->reflection = specular[1];
	new_object->refraction = specular[2];
	new_object->transparency = specular[3];
	new_object->t_scale = specular[4];
	new_object->cs_nmb = 0;
	new_object->surface_id = specular[5];
	new_object->cutting_surfaces = NULL;
	new_object->color = color;
	new_object->data = (void *)new_plane;
	new_object->tag = "plane";
	new_object->type = PLANE;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->intersect = &intersect_ray_plane;
	new_object->get_normal = &get_plane_normal;
	new_object->clear_obj = &clear_default;
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
}

t_object	*new_plane(cl_float3 *poi_nor, float *specular, t_color color)
{
	t_plane		*new_plane;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_plane = protected_malloc(sizeof(t_plane), 1);
	new_plane->normal = poi_nor[1];
	normalize_vector(&new_plane->normal);
	new_plane->point = poi_nor[0];
	plane_init(new_object, poi_nor, specular, color);
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_plane->normal, matrix, 1);
	matr_free(matrix, 4);
	new_plane->d = -new_plane->normal.x * new_plane->point.x - new_plane->\
	normal.y * new_plane->point.y - new_plane->normal.z * new_plane->point.z;
	new_object->basis = get_plane_default(new_object->basis, new_plane->normal);
	return (new_object);
}

void		set_arg2_plane(t_scene *scene, int index, \
								int is_refractive, cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[5], 6, \
	sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[5], 7, \
	sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[5], 8, \
	sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[5], 9, \
	sizeof(cl_mem), &scene->cl_data.scene.material_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 10, \
	sizeof(cl_int), (void*)&is_refractive);
	clSetKernelArg(scene->cl_data.kernels[5], 11, \
	sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 12, \
	sizeof(cl_float), (void*)&scene->objs[index]->refraction);
	clSetKernelArg(scene->cl_data.kernels[5], 13, \
	sizeof(cl_mem), &scene->cl_data.scene.exception_buf);
}

void		set_arg_plane(t_scene *scene, int index)
{
	clSetKernelArg(scene->cl_data.kernels[5], 0, \
	sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 1, \
	sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 2, \
	sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 3, \
	sizeof(t_plane), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[5], 4, \
	sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 5, \
	sizeof(cl_int), (void*)&index);
}
