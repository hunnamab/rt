/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 01:30:46 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 01:31:14 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		paraboloid_init(t_object *new_object, cl_float3 *cen_buf, \
							t_color color, int surface_id)
{
	new_object->rotation[0] = cen_buf[2].x;
	new_object->rotation[1] = cen_buf[2].y;
	new_object->rotation[2] = cen_buf[2].z;
	new_object->position = cen_buf[0];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->type = PARABOLOID;
	new_object->tag = "paraboloid";
	new_object->intersect = &intersect_ray_paraboloid;
	new_object->clear_obj = &clear_default;
}

t_object	*new_paraboloid(cl_float3 *cen_buf, t_color color, \
								float *specular, int surface_id)
{
	t_paraboloid	*parab;
	t_object		*new_object;

	new_object = malloc(sizeof(t_object));
	parab = malloc(sizeof(t_paraboloid));
	parab->center = cen_buf[0];
	parab->vec = cen_buf[1];
	parab->k = specular[0];
	new_object->specular = specular[1];
	new_object->reflection = specular[2];
	new_object->refraction = specular[3];
	new_object->transparency = specular[4];
	new_object->data = (void *)parab;
	paraboloid_init(new_object, cen_buf, color, surface_id);
	return (new_object);
}
