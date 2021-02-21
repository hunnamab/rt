/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 14:44:06 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/21 14:58:46 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			init_norme(int *norme)
{
	norme[0] = 0;
	norme[1] = 0;
	norme[2] = -1;
	norme[3] = 0;
}

static void			split_light(int *scij, t_scene *scene, char *buf)
{
	if (buf[scij[2] + 1] == '[')
	{
		while (buf[scij[2]] != ']')
		{
			if (buf[scij[2]] == '{')
				scene->light_nmb++;
			scij[2]++;
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2] - 1]))
				scij[2]++;
		}
	}
	else
		scene->light_nmb++;
}

static void			split_else(int *scij, char *buf)
{
	if (buf[scij[2] + 1] == '{')
	{
		while (buf[scij[2]] != '}')
			scij[2]++;
	}
	if (buf[scij[2] + 1] == '[')
	{
		while (buf[scij[2]] != ']')
		{
			scij[2]++;
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2] - 1]))
				scij[2]++;
		}
	}
}

static void			split_surface(int *scij, t_scene *scene, char *buf)
{
	if (buf[scij[2] + 1] == '[')
	{
		while (buf[scij[2]] != ']')
		{
			if (buf[scij[2]] == '{')
				scene->srf_nmb++;
			scij[2]++;
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2] - 1]))
				scij[2]++;
		}
	}
	else
		scene->srf_nmb++;
}

void				split_objects(int len, t_scene *scene, char *buf)
{
	char			*obn;
	int				scij[4];

	init_norme(scij);
	while (++scij[2] < len)
	{
		if ((buf[scij[2] + 1] == '{' || buf[scij[2] + 1] == '[')
				&& scij[3] < scene->obj_nmb)
		{
			if (!(obn = ft_strsub(buf, scij[0], (scij[2] - scij[0] - 1))))
				output_error(6);
			strequal(obn, scij, scene, buf);
			ft_memdel((void **)&obn);
			split_else(scij, buf);
			scij[0] = scij[2] + 3;
			scij[3]++;
		}
	}
	scene->box_nmb *= 5;
	scene->obj_nmb += scene->box_nmb;
	scene->obj_nmb = scene->obj_nmb - scene->light_nmb
			- scene->srf_nmb - scij[1];
	if (scene->obj_nmb < 1 || scene->light_nmb < 1 || scij[1] != 1)
		output_error(0);
}
