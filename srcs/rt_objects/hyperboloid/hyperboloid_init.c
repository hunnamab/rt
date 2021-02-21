/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 01:16:22 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 01:17:58 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		hyperboloid_init(t_object *new_object, cl_float3 *cen_buf,
								t_color color, int surface_id)
{
	new_object->rotation[0] = cen_buf[1].x;
	new_object->rotation[1] = cen_buf[1].y;
	new_object->rotation[2] = cen_buf[1].z;
	new_object->position = cen_buf[0];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->type = HYPERBOLOID;
	new_object->tag = "hyperboloid";
	new_object->intersect = &intersect_ray_hyperboloid;
	new_object->clear_obj = &clear_default;
}

t_object	*new_hyperboloid(cl_float3 *cen_buf, \
		float *specular, t_color color, int surface_id)
{
	t_hyperboloid	*hyper;
	t_object		*new_object;

	new_object = malloc(sizeof(t_object));
	hyper = malloc(sizeof(t_hyperboloid));
	hyper->center = cen_buf[0];
	hyper->a = specular[0];
	hyper->b = specular[1];
	hyper->c = specular[2];
	new_object->specular = specular[3];
	new_object->reflection = specular[4];
	new_object->refraction = specular[5];
	new_object->transparency = specular[6];
	new_object->data = (void *)hyper;
	hyperboloid_init(new_object, cen_buf, color, surface_id);
	return (new_object);
}
