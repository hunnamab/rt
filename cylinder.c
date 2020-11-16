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

#include "rtv1.h"

t_object	*new_cylinder(t_point *pos_vec, double *rad_spec, t_color color, \
							double *rotation)
{
	t_cylinder	*new_cylinder;
	t_object	*new_object;
	double		**matrix;

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
	new_object->intersect = &intersect_ray_cylinder;
	new_object->get_normal = &get_cylinder_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void		get_cylinder_normal(t_scene *scene, int index, int obj_num)
{
	t_cylinder	*cylinder;
	t_point		*normal;
	double		m;
	t_point		p;
	t_point		buf[2];

	normal = &scene->normal_buf[index];
	cylinder = (t_cylinder *)scene->objs[obj_num]->data;
	buf[0] = vector_sub(&scene->ray_buf[index].start, &cylinder->position);
	m = vector_dot(&scene->ray_buf[index].dir, &cylinder->vec) * \
		scene->depth_buf[index] + vector_dot(&buf[0], &cylinder->vec);
	buf[0] = vector_scale(&scene->ray_buf[index].dir, scene->depth_buf[index]);
	p = vector_add(&scene->ray_buf[index].start, &buf[0]);
	buf[0] = vector_sub(&p, &cylinder->position);
	buf[1] = vector_scale(&cylinder->vec, m);
	*normal = vector_sub(&buf[0], &buf[1]);
	scene->normal_buf[index] = vector_div_by_scalar(&scene->normal_buf[index], \
								vector_length(&scene->normal_buf[index]));
	if (vector_dot(&scene->ray_buf[index].dir, normal) > 0.0001)
		*normal = vector_scale(normal, -1);
}

double		intersect_ray_cylinder(t_ray *r, t_object *object)
{
	double		a;
	double		b;
	double		c;
	t_point		dist;
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)object->data;
	dist = vector_sub(&r->start, &cylinder->position);
	a = vector_dot(&r->dir, &cylinder->vec);
	a = vector_dot(&r->dir, &r->dir) - a * a;
	b = 2 * (vector_dot(&r->dir, &dist) - \
				vector_dot(&r->dir, &cylinder->vec) * \
				vector_dot(&dist, &cylinder->vec));
	c = vector_dot(&dist, &cylinder->vec);
	c = vector_dot(&dist, &dist) - c * c - cylinder->radius * cylinder->radius;
	c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		return (choose_t((-b + c) / (2 * a), (-b - c) / (2 * a)));
	}
	return (0);
}
