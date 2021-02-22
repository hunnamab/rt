/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 02:19:52 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 16:36:01 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			sphere_init(t_object *new_object, float *rotation, \
											t_color color, cl_float3 center)
{
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	new_object->position = center;
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->tag = "sphere";
	new_object->type = SPHERE;
	new_object->intersect = &intersect_ray_sphere;
	new_object->get_normal = &get_sphere_normal;
	new_object->clear_obj = &clear_default;
	new_object->cs_nmb = 0;
	new_object->cutting_surfaces = NULL;
	new_object->basis = get_default(new_object->basis);
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
}

t_object		*new_sphere(cl_float3 center, float *rad_spec, \
									t_color color, float *rotation)
{
	t_sphere *new_sphere;
	t_object *new_object;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_sphere = protected_malloc(sizeof(t_sphere), 1);
	new_sphere->center = center;
	new_sphere->radius = rad_spec[0];
	new_object->specular = rad_spec[1];
	new_object->reflection = rad_spec[2];
	new_object->refraction = rad_spec[3];
	new_object->transparency = rad_spec[4];
	new_object->t_scale = rad_spec[5];
	new_object->surface_id = rad_spec[6];
	sphere_init(new_object, rotation, color, center);
	new_object->data = (void *)new_sphere;
	ft_printf("new sphere radius %f\n", new_sphere->radius);
	return (new_object);
}

void			set_arg2_sphere(t_scene *scene, int index, int is_refractive)
{
	clSetKernelArg(scene->cl_data.kernels[1], 7, \
	sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[1], 8, \
	sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[1], 9, \
	sizeof(cl_mem), &scene->cl_data.scene.material_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 10, \
	sizeof(cl_int), (void*)&is_refractive);
	clSetKernelArg(scene->cl_data.kernels[1], 11, \
	sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 12, \
	sizeof(cl_float), (void*)&scene->objs[index]->refraction);
	clSetKernelArg(scene->cl_data.kernels[1], 13, \
	sizeof(cl_mem), &scene->cl_data.scene.exception_buf);
}

void			set_arg_sphere(t_scene *scene, int index, cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[1], 0, \
	sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 1, \
	sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 2, \
	sizeof(t_sphere), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[1], 3, \
	sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 4, \
	sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 5, \
	sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[1], 6, \
	sizeof(cl_mem), &cs);
}
