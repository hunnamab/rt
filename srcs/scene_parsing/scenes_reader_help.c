/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader_help.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 14:33:27 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/21 14:41:37 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			init_norme_2(int *snmi)
{
	snmi[0] = 2;
	snmi[1] = 0;
	snmi[2] = 0;
	snmi[3] = 0;
	snmi[4] = 0;
}

static	void		texts_n_objects(t_scene *scene, int txt_nmb)
{
	int				i;
	int				j;

	scene->texts = protected_malloc(sizeof(t_texture *), txt_nmb + 1);
	j = 0;
	i = 0;
	while (j < scene->obj_nmb)
	{
		if (scene->objs[j]->text != NULL)
		{
			scene->texts[i] = scene->objs[j]->text;
			scene->objs[j]->texture_id = i;
			i++;
		}
		if (scene->objs[j]->normal_text != NULL)
		{
			scene->texts[i] = scene->objs[j]->normal_text;
			scene->objs[j]->normal_map_id = i;
			i++;
		}
		j++;
	}
}

static void			scene_malloc(t_scene *scene)
{
	scene->srfs = protected_malloc(sizeof(t_cutting_surface), scene->srf_nmb);
	scene->objs = protected_malloc(sizeof(t_object *), scene->obj_nmb);
	scene->light = protected_malloc(sizeof(t_light), scene->light_nmb);
}

static void			do_texts(t_scene *scene, int txt_nmb)
{
	int				i;

	i = 0;
	while (i < scene->obj_nmb)
	{
		if (scene->objs[i]->text != NULL)
			txt_nmb++;
		if (scene->objs[i]->normal_text != NULL)
			txt_nmb++;
		i++;
	}
	texts_n_objects(scene, txt_nmb);
}

void				get_objects(char *buf, t_scene *scene, int len)
{
	int				snmi[5];
	char			c;
	int				txt_nmb;

	txt_nmb = 0;
	init_norme_2(snmi);
	scene->obj_nmb = count_objects(len, buf);
	split_objects(len, scene, buf);
	scene_malloc(scene);
	while (snmi[3] < len)
	{
		if (buf[snmi[3] + 1] == '{' || buf[snmi[3] + 1] == '[')
		{
			c = buf[snmi[3] + 1];
			scene_objects(snmi, scene, buf);
		}
		snmi[3]++;
	}
	do_texts(scene, txt_nmb);
	free(buf);
}
