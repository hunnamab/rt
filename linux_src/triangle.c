/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 21:42:26 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:36:19 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_cuda.h"

static void	init_norme(float *r, float3 *v, float *rt, float3 *vt)
{
	rt[0] = r[0];
	rt[1] = r[1];
	rt[2] = r[2];
	vt[0] = v[0];
	vt[1] = v[1];
	vt[2] = v[2];
}

t_object	*new_triangle(float3 *vertex, float specular, t_color color, \
							float *rotation)
{
	t_triangle	*new_triangle;
	t_object	*new_object;
	float3		edge1;
	float3		edge2;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_triangle = protected_malloc(sizeof(t_triangle), 1);
	new_triangle->vertex = protected_malloc(sizeof(float3), 3);
	init_norme(rotation, vertex, new_object->rotation, new_triangle->vertex);
	matrix = get_rotation_matrix(new_object->rotation);
	transform(new_triangle->vertex, matrix, 3);
	matr_free(matrix, 4);
	edge1 = vector_sub(&new_triangle->vertex[0], &new_triangle->vertex[1]);
	edge2 = vector_sub(&new_triangle->vertex[1], &new_triangle->vertex[2]);
	new_triangle->normal = vector_cross(&edge1, &edge2);
	normalize_vector(&new_triangle->normal);
	new_object->type = TRIANGLE;
	new_object->specular = specular;
	new_object->color = color;
	new_object->data = (void *)new_triangle;
	new_object->tag = "triangle";
	new_object->clear_obj = &clear_triangle;
	new_object->intersect = &intersect_ray_triangle;
	return (new_object);
}

void		clear_triangle(t_object *obj)
{
	t_triangle *t;

	t = (t_triangle *)obj->data;
	free(t->vertex);
	free(obj->data);
	free(obj);
}
