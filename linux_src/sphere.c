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

#include "rt_cuda.h"
#include "rt.cuh"
t_object	*new_sphere(float3 center, float *rad_spec, t_color color, \
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
	new_object->data_size = sizeof(t_sphere);
	new_object->tag = "sphere";
	new_object->type = SPHERE;
	new_object->clear_obj = &clear_default;
	new_object->intersect = &intersect_ray_sphere;
	return (new_object);
}
