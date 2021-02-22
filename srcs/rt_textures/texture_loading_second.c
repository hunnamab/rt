/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loading_second.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 17:32:55 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/22 17:33:23 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			norme(t_scene *scene, cl_int4 *index_buf, \
long long int *ijiss, unsigned char *texture_buf)
{
	scene->shift_buf[ijiss[0]].x = ijiss[3];
	scene->objs[ijiss[0]]->texture_id = scene->shift_buf[ijiss[0]].x;
	ijiss[3] += scene->objs[ijiss[0]]->text->size;
	ft_memcpy(texture_buf, scene->objs[ijiss[0]]->text->pixels, \
	scene->objs[ijiss[0]]->text->size);
	texture_buf += scene->objs[ijiss[0]]->text->size;
}

static void			norme_second(t_scene *scene, \
cl_int4 *index_buf, long long int *ijiss)
{
	while (ijiss[2] != index_buf[ijiss[1]].s1)
		ijiss[1]++;
	scene->objs[ijiss[0]]->texture_id = \
	scene->shift_buf[index_buf[ijiss[1]].s0].x;
	ijiss[1] = 0;
}

static	void		norme_third(t_scene *scene, \
cl_int4 *index_buf, long long int *ijiss, unsigned char *texture_buf)
{
	scene->shift_buf[ijiss[0]].y = ijiss[3];
	scene->objs[ijiss[0]]->normal_map_id = scene->shift_buf[ijiss[0]].y;
	ijiss[3] += scene->objs[ijiss[0]]->normal_text->size;
	ft_memcpy(texture_buf, scene->objs[ijiss[0]]->\
		normal_text->pixels, scene->objs[ijiss[0]]->normal_text->size);
}

static	void		norme_fourth(t_scene *scene, \
cl_int4 *index_buf, long long int *ijiss)
{
	while (ijiss[2] != index_buf[ijiss[1]].s3)
		ijiss[1]++;
	scene->objs[ijiss[0]]->normal_map_id = \
	scene->shift_buf[index_buf[ijiss[1]].s2].y;
	ijiss[1] = 0;
}

void				get_texture_buf(t_scene *scene, cl_int4 *index_buf, \
unsigned char *texture_buf, long long int *ijiss)
{
	while (++ijiss[0] < scene->obj_nmb)
	{
		if (scene->objs[ijiss[0]]->text != NULL)
		{
			if ((ijiss[2] = is_copy(scene, \
			scene->objs[ijiss[0]]->texture_id)) < 0)
			{
				norme(scene, index_buf, ijiss, texture_buf);
				texture_buf += scene->objs[ijiss[0]]->text->size;
			}
			else
				norme_second(scene, index_buf, ijiss);
		}
		if (scene->objs[ijiss[0]]->normal_text != NULL)
		{
			if ((ijiss[2] = is_copy(scene, \
			scene->objs[ijiss[0]]->normal_map_id)) < 0)
			{
				norme_third(scene, index_buf, ijiss, texture_buf);
				texture_buf += scene->objs[ijiss[0]]->normal_text->size;
			}
			else
				norme_fourth(scene, index_buf, ijiss);
		}
	}
}
