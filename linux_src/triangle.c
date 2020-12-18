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
	new_object->specular = specular;
	new_object->color = color;
	new_object->data = (void *)new_triangle;
	new_object->tag = "triangle";
	new_object->intersect = &intersect_ray_triangle;
	new_object->get_normal = &get_triangle_normal;
	new_object->clear_obj = &clear_triangle;
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

void		get_triangle_normal(t_scene *scene, int index, int obj_num)
{
	t_triangle *t;

	t = (t_triangle *)scene->objs[obj_num]->data;
	copy_point(&scene->normal_buf[index], &t->normal);
	if (vector_dot(&scene->ray_buf[index], \
	&scene->normal_buf[index]) > 0.0001)
		scene->normal_buf[index] = vector_scale(&scene->normal_buf[index], -1);
}

float		intersect_ray_triangle(t_scene *scene, int index, float3 *start, float3 *dir)
{
	t_triangle	*triangle;
	float3		edge[2];
	float3		vec[3];
	float		det;
	float		uv[2];

	triangle = (t_triangle *)scene->objs[index]->data;
	edge[0] = vector_sub(&triangle->vertex[1], &triangle->vertex[0]);
	edge[1] = vector_sub(&triangle->vertex[2], &triangle->vertex[0]);
	vec[0] = vector_cross(dir, &edge[1]);
	det = vector_dot(&edge[0], &vec[0]);
	if (det < 1e-8 && det > -1e-8)
		return (0);
	det = 1 / det;
	vec[1] = vector_sub(start, &triangle->vertex[0]);
	uv[0] = vector_dot(&vec[1], &vec[0]) * det;
	if (uv[0] < 0 || uv[0] > 1)
		return (0);
	vec[2] = vector_cross(&vec[1], &edge[0]);
	uv[1] = vector_dot(dir, &vec[2]) * det;
	if (uv[1] < 0 || uv[0] + uv[1] > 1)
		return (0);
	if ((vector_dot(&edge[1], &vec[2]) * det) > 0)
		return (vector_dot(&edge[1], &vec[2]) * det);
	return (0);
}
