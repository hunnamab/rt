/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:39 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 15:14:07 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_cuda.h"

t_object	*new_cone(float3 *pos_vec, float *ang_spec, t_color color, \
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
	new_object->color = color;
	new_object->data = (void *)new_cone;
	new_object->tag = "cone";
	new_object->type = CONE;
	new_object->intersect = &intersect_ray_cone;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

