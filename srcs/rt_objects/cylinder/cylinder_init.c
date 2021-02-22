/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:57:38 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/22 18:59:08 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			cyl_init(t_object *new_object, float *rad_spec, t_color color)
{
	new_object->cs_nmb = 0;
	new_object->surface_id = rad_spec[6];
	new_object->cutting_surfaces = NULL;
	new_object->color = color;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->tag = "cylinder";
	new_object->type = CYLINDER;
	new_object->intersect = &intersect_ray_cylinder;
	new_object->clear_obj = &clear_default;
	new_object->basis = get_default(new_object->basis);
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
}

t_object		*new_cylinder(cl_float3 *pos_vec, float *rad_spec,
															t_color color)
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
	new_object->reflection = rad_spec[2];
	new_object->refraction = rad_spec[3];
	new_object->transparency = rad_spec[4];
	new_object->t_scale = rad_spec[5];
	new_object->position = pos_vec[0];
	new_object->rotation[0] = pos_vec[2].x;
	new_object->rotation[1] = pos_vec[2].y;
	new_object->rotation[2] = pos_vec[2].z;
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_cylinder->vec, matrix, 1);
	matr_free(matrix, 4);
	cyl_init(new_object, rad_spec, color);
	new_object->data = (void *)new_cylinder;
	return (new_object);
}
