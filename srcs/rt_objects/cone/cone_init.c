/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 00:39:21 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 16:32:49 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		init_objects(t_object *new_object, cl_float3 *pos_vec, \
											float *ang_spec, t_color color)
{
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
	new_object->tag = "cone";
	new_object->type = CONE;
	new_object->intersect = &intersect_ray_cone;
	new_object->get_normal = &get_cone_normal;
	new_object->clear_obj = &clear_default;
	new_object->basis = get_default(new_object->basis);
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
}

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
	init_objects(new_object, pos_vec, ang_spec, color);
	new_object->data = (void *)new_cone;
	return (new_object);
}
