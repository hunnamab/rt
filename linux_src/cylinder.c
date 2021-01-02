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

#include "rt_cuda.h"

t_object	*new_cylinder(float3 *pos_vec, float *rad_spec, t_color color, \
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
	new_object->clear_obj = &clear_default;
	new_object->intersect = &intersect_ray_cylinder;
	return (new_object);
}
