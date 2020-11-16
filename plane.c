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

#include "rtv1.h"

t_object	*new_plane(t_point *poi_nor, double specular, t_color color, \
						double *rotation)
{
	t_plane		*new_plane;
	t_object	*new_object;
	double		**matrix;

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
	new_object->intersect = &intersect_ray_plane;
	new_object->get_normal = &get_plane_normal;
	new_plane->d = -new_plane->normal.x * new_plane->point.x - new_plane->\
	normal.y * new_plane->point.y - new_plane->normal.z * new_plane->point.z;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void		get_plane_normal(t_scene *scene, int index, int obj_num)
{
	t_plane *p;

	p = (t_plane *)scene->objs[obj_num]->data;
	copy_point(&scene->normal_buf[index], &p->normal);
	if (vector_dot(&scene->ray_buf[index].dir, \
		&scene->normal_buf[index]) > 0.0001)
		scene->normal_buf[index] = vector_scale(&scene->normal_buf[index], -1);
}

double		intersect_ray_plane(t_ray *r, t_object *object)
{
	double	k1;
	double	k2;
	t_plane	*plane;

	plane = (t_plane *)object->data;
	if ((vector_dot(&r->dir, &plane->normal)) == 0)
		return (0);
	k1 = vector_dot(&r->start, &plane->normal) + plane->d;
	k2 = vector_dot(&r->dir, &plane->normal);
	if (k1 == 0 || (k1 < 0 && k2 < 0) || (k1 > 0 && k2 > 0))
		return (0);
	return (-k1 / k2);
}
