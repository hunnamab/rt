/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:22:24 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:59:39 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "rt_cuda.h"

t_object	*new_plane(float3 *poi_nor, float specular, t_color color, \
						float *rotation)
{
	t_plane		*new_plane;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_plane = protected_malloc(sizeof(t_plane), 1);
	new_plane->normal = poi_nor[1];
	normalize_vector(&new_plane->normal);
	new_plane->point = poi_nor[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_plane->normal, matrix, 1);
	matr_free(matrix, 4);
	new_object->specular = specular;
	new_object->color = color;
	new_object->data = (void *)new_plane;
	new_object->tag = "plane";
	new_object->type = PLANE;
	new_plane->d = -new_plane->normal.x * new_plane->point.x - new_plane->\
	normal.y * new_plane->point.y - new_plane->normal.z * new_plane->point.z;
	new_object->clear_obj = &clear_default;
	new_object->intersect = &intersect_ray_plane;
	return (new_object);
}
