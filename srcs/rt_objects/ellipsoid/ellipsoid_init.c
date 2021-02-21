/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 01:00:08 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 01:00:54 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		init_ell(t_object *new_object, cl_float3 *buf, \
					t_color color, int surface_id)
{
	new_object->rotation[0] = buf[1].x;
	new_object->rotation[1] = buf[1].y;
	new_object->rotation[2] = buf[1].z;
	new_object->position = buf[0];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->type = ELLIPSOID;
	new_object->tag = "ellipsoid";
	new_object->intersect = &intersect_ray_ellipsoid;
	new_object->clear_obj = &clear_default;
}

t_object	*new_ellipsoid(cl_float3 *buf, t_color color, \
								float *specular, int surface_id)
{
	t_ellipsoid	*el;
	t_object	*new_object;

	new_object = malloc(sizeof(t_object));
	el = malloc(sizeof(t_ellipsoid));
	el->center = buf[0];
	el->a = specular[0];
	el->b = specular[1];
	el->c = specular[2];
	new_object->data = (void *)el;
	new_object->specular = specular[3];
	new_object->reflection = specular[4];
	new_object->refraction = specular[5];
	new_object->transparency = specular[6];
	init_ell(new_object, buf, color, surface_id);
	return (new_object);
}
