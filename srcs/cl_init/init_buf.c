/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_buf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 03:17:09 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 03:18:17 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_buf3(t_scene *scene, t_object_d *buf, int i)
{
	buf[i].type = scene->objs[i]->type;
	buf[i].rotation = get_point(0, 0, 0);
	buf[i].specular = scene->objs[i]->specular;
	buf[i].color = scene->objs[i]->color;
	buf[i].roughness = 0;
	buf[i].color_disrupt = 0;
	buf[i].refraction = scene->objs[i]->refraction;
	buf[i].reflection = scene->objs[i]->reflection;
	buf[i].transparency = scene->objs[i]->transparency;
	if (scene->objs[i]->t_scale)
		buf[i].t_scale = scene->objs[i]->t_scale;
	else
		buf[i].t_scale = 1;
	buf[i].basis = scene->objs[i]->basis;
}

void	init_buf2(t_scene *scene, t_object_d *buf, int i)
{
	if (scene->objs[i]->normal_text != NULL)
	{
		buf[i].normal_map_id = scene->objs[i]->normal_map_id;
		buf[i].texture_size_nm = scene->objs[i]->normal_text->size;
		buf[i].texture_width_nm = scene->objs[i]->normal_text->width;
		buf[i].texture_height_nm = scene->objs[i]->normal_text->height;
		buf[i].l_size_nm = scene->objs[i]->normal_text->l_size;
	}
	else
	{
		buf[i].normal_map_id = -1;
		buf[i].texture_size_nm = -1;
	}
}

void	init_buf(t_scene *scene, t_object_d *buf, int i)
{
	if (scene->objs[i]->text != NULL)
	{
		buf[i].texture_id = scene->objs[i]->texture_id;
		buf[i].texture_size = scene->objs[i]->text->size;
		buf[i].texture_width = scene->objs[i]->text->width;
		buf[i].texture_height = scene->objs[i]->text->height;
		buf[i].l_size = scene->objs[i]->text->l_size;
	}
	else
	{
		buf[i].texture_id = -1;
		buf[i].texture_size = -1;
	}
	init_buf2(scene, buf, i);
	init_buf3(scene, buf, i);
}
