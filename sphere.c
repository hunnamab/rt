/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 22:45:20 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 15:03:07 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_object	*new_sphere(cl_float3 center, float *rad_spec, t_color color, \
						float *rotation)
{
	t_sphere *new_sphere;
	t_object *new_object;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_sphere = protected_malloc(sizeof(t_sphere), 1);
	new_sphere->center = center;
	new_sphere->radius = rad_spec[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	new_object->specular = rad_spec[1];
	new_object->color = color;
	new_object->data = (void *)new_sphere;
	new_object->tag = "sphere";
	new_object->intersect = &intersect_ray_sphere;
	new_object->get_normal = &get_sphere_normal;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void		get_sphere_normal(t_scene *scene, int index, int obj_num)
{
	t_sphere *s;

	s = (t_sphere *)scene->objs[obj_num]->data;
	scene->normal_buf[index] = \
	vector_sub(&scene->intersection_buf[index], &s->center);
	scene->normal_buf[index] = vector_div_by_scalar(&scene->normal_buf[index], \
	vector_length(&scene->normal_buf[index]));
	if (vector_dot(&scene->ray_buf[index], \
	&scene->normal_buf[index]) > 0.0001)
		scene->normal_buf[index] = vector_scale(&scene->normal_buf[index], -1);
}

float		intersect_ray_sphere(t_scene *scene, int index, cl_float3 *start, cl_float3 *dir)
{
	float		a;
	float		b;
	float		c;
	cl_float3	dist;
	t_sphere	*s;
	
	s = (t_sphere *)scene->objs[index]->data;
	a = vector_dot(dir, dir);
	dist = vector_sub(start, &s->center);
	b = 2 * vector_dot(&dist, dir);
	c = vector_dot(&dist, &dist) - (s->radius * s->radius);
	c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		return (choose_t((-b + c) / (2 * a), (-b - c) / (2 * a)));
	}
	return (0);
}
