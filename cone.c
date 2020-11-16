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

#include "rtv1.h"

t_object	*new_cone(t_point *pos_vec, double *ang_spec, t_color color, \
						double *rotation)
{
	t_cone		*new_cone;
	t_object	*new_object;
	double		**matrix;

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
	new_object->intersect = &intersect_ray_cone;
	new_object->get_normal = &get_cone_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void		get_cone_normal(t_scene *scene, int index, int obj_num)
{
	t_cone	*cone;
	double	m;
	t_point	*normal;
	t_point buf;

	normal = &scene->normal_buf[index];
	cone = (t_cone *)scene->objs[obj_num]->data;
	buf = vector_sub(&scene->ray_buf[index].start, &cone->position);
	m = vector_dot(&scene->ray_buf[index].dir, &cone->vec) * \
					scene->depth_buf[index] + vector_dot(&buf, &cone->vec);
	buf = vector_scale(&cone->vec, m);
	*normal = vector_scale(&buf, (1 + cone->angle * cone->angle));
	buf = vector_sub(&scene->intersection_buf[index], &cone->position);
	*normal = vector_sub(&buf, normal);
	scene->normal_buf[index] = vector_div_by_scalar(&scene->normal_buf[index], \
								vector_length(&scene->normal_buf[index]));
	if (vector_dot(&scene->ray_buf[index].dir, normal) > 0.0001)
		*normal = vector_scale(normal, -1);
}

double		intersect_ray_cone(t_ray *r, t_object *object)
{
	double	a;
	double	b;
	double	c;
	t_point	dist;
	t_cone	*cone;

	cone = (t_cone *)object->data;
	dist = vector_sub(&r->start, &cone->position);
	a = vector_dot(&r->dir, &cone->vec);
	a = vector_dot(&r->dir, &r->dir) - (1 + cone->angle * cone->angle) * a * a;
	b = 2 * (vector_dot(&r->dir, &dist) - (1 + cone->angle * cone->angle) * \
		vector_dot(&r->dir, &cone->vec) * vector_dot(&dist, &cone->vec));
	c = vector_dot(&dist, &cone->vec);
	c = vector_dot(&dist, &dist) - (1 + cone->angle * cone->angle) * c * c;
	c = b * b - 4 * a * c;
	c = DROUND(c);
	if (c >= 0)
	{
		c = sqrt(c);
		return (choose_t((-b + c) / (2 * a), (-b - c) / (2 * a)));
	}
	return (0);
}
