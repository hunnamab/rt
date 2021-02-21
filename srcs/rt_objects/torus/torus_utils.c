/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 02:35:29 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 02:36:14 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		set_arg2_torus(t_scene *scene, int index, \
						int is_refractive)
{
	clSetKernelArg(scene->cl_data.kernels[13], 7, \
	sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[13], 8, \
	sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[13], 9, \
	sizeof(cl_mem), &scene->cl_data.scene.material_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 10, \
	sizeof(cl_int), (void*)&is_refractive);
	clSetKernelArg(scene->cl_data.kernels[13], 11, \
	sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 12, \
	sizeof(cl_float), (void*)&scene->objs[index]->refraction);
	clSetKernelArg(scene->cl_data.kernels[13], 13, \
	sizeof(cl_mem), &scene->cl_data.scene.exception_buf);
}

void		set_arg_torus(t_scene *scene, int index, \
						cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[13], 0, \
	sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 1, \
	sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 2, \
	sizeof(t_torus), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[13], 3, \
	sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 4, \
	sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[13], 5, \
	sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[13], 6, \
	sizeof(cl_mem), &cs);
}

void		torus_init(t_object *new_object, cl_float3 *cen_buf, \
								t_color color, int surface_id)
{
	new_object->rotation[0] = cen_buf[2].x;
	new_object->rotation[1] = cen_buf[2].y;
	new_object->rotation[2] = cen_buf[2].z;
	new_object->position = cen_buf[0];
	new_object->color = color;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->type = TORUS;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->intersect = &intersect_ray_torus;
	new_object->clear_obj = &clear_default;
}

t_object	*new_torus(cl_float3 *cen_buf, t_color color, \
							float *specular, int surface_id)
{
	t_torus			*torus;
	t_object		*new_object;

	new_object = malloc(sizeof(t_object));
	torus = malloc(sizeof(t_torus));
	torus->center = cen_buf[0];
	torus->vec = cen_buf[1];
	torus->radius1 = specular[0];
	torus->radius2 = specular[1];
	new_object->specular = specular[2];
	new_object->reflection = specular[3];
	new_object->refraction = specular[4];
	new_object->transparency = specular[5];
	new_object->data = (void *)torus;
	torus_init(new_object, cen_buf, color, surface_id);
	return (new_object);
}
