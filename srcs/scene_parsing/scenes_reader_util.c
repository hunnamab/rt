/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 14:44:06 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/20 21:41:40 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	description_helper(char *scene, int *len)
{
	if (scene[len[1]] == '[')
	{
		while (scene[len[1] + 1] != ']')
		{
			len[1]++;
			len[0]++;
			if (scene[len[1] + 1] == ']' && ft_isdigit(scene[len[1]]))
			{
				len[1]++;
				len[0]++;
			}
		}
	}
	else
	{
		while (scene[len[1] + 1] != '}')
		{
			len[1]++;
			len[0]++;
		}
	}
}

char		**get_description(char *scene, int i)
{
	char	*descr_buf;
	char	**description;
	int		len[2];
	int		start;

	len[0] = 0;
	i = i - 1;
	start = i;
	len[1] = i;
	description_helper(scene, len);
	if (scene[len[1]] != '\t')
		output_error(6);
	if (!(descr_buf = ft_strsub(scene, start, len[0] + 2)))
		output_error(6);
	if (!(description = ft_strsplit(descr_buf, '\n')))
		output_error(6);
	ft_memdel((void **)&descr_buf);
	return (description);
}

int			count_objects(int len, char *buf)
{
	int i;
	int obj_nmb;

	i = 0;
	obj_nmb = 0;
	while (i < len && buf[i])
	{
		if (buf[i] == '{')
			obj_nmb++;
		i++;
	}
	if (obj_nmb == 0)
		output_error(0);
	return (obj_nmb - 1);
}

static void	init_norme(int *norme)
{
	norme[0] = 0;
	norme[1] = 0;
	norme[2] = -1;
	norme[3] = 0;
}

static void split_light(int *scij, t_scene *scene,char *buf)
{
	if (buf[scij[2] + 1] == '[')
	{
		while (buf[scij[2]] != ']')
		{
			if (buf[scij[2]] == '{')
				scene->light_nmb++;
			scij[2]++;
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2]- 1]))
				scij[2]++;
		}
	}
	else
		scene->light_nmb++;
}

static void	split_else(int *scij, char *buf)
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
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2]- 1]))
				scij[2]++;
		}
	}
}

static void split_surface(int *scij, t_scene *scene,char *buf)
{
	if (buf[scij[2] + 1] == '[')
	{
		while (buf[scij[2]] != ']')
		{
			if (buf[scij[2]] == '{')
				scene->srf_nmb++;
			scij[2]++;
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2]- 1]))
				scij[2]++;
		}
	}
	else
		scene->srf_nmb++;
}

static void split_box(int *scij, t_scene *scene,char *buf)
{
	if (buf[scij[2] + 1] == '[')
	{
		while (buf[scij[2]] != ']')
		{
			if (buf[scij[2]] == '{')
				scene->box_nmb++;
			scij[2]++;
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2]- 1]))
				scij[2]++;
		}
	}
	else
		scene->box_nmb++;
}


void		split_objects(int len, t_scene *scene, char *buf)
{
	char	*obj_name;
	int		scij[4];
	 // start, camera, i, j
	int i = 0;

	init_norme(scij);
	while (++scij[2] < len)
	{
		if ((buf[scij[2] + 1] == '{' || buf[scij[2] + 1] == '[') && scij[3] < scene->obj_nmb)
		{
			if (!(obj_name = ft_strsub(buf, scij[0], (scij[2] - scij[0] - 1))))
				output_error(6);
			if (ft_strequ(obj_name, "\t\"light\"") || ft_strequ(obj_name, "{\n\t\"light\""))
				split_light(scij, scene, buf);
			if (ft_strequ(obj_name, "\t\"surface\"") || ft_strequ(obj_name, "{\n\t\"surface\""))
				split_surface(scij, scene, buf);
			if (ft_strequ(obj_name, "\t\"box\"") || ft_strequ(obj_name, "{\n\t\"box\""))
				split_box(scij, scene, buf);
			if (ft_strequ(obj_name, "\t\"camera\"") || ft_strequ(obj_name, "{\n\t\"camera\""))
				scij[1]++;
			ft_memdel((void **)&obj_name);
			split_else(scij, buf);
			scij[0] = scij[2] + 3; 
			scij[3]++;
		}
	}
	printf("\nLOLO%d\n", scene->box_nmb);
	scene->box_nmb *= 5;
	scene->obj_nmb += scene->box_nmb;
	scene->obj_nmb = scene->obj_nmb - scene->light_nmb - scene->srf_nmb - scij[1];
	printf("\n\nobj_nmb %d srf_nmb %d\n\n", scene->obj_nmb, scene->srf_nmb);
	if (scene->obj_nmb < 1 || scene->light_nmb < 1 || scij[1] != 1)
		output_error(0);
}
