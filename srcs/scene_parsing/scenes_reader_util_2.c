/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader_util_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 14:48:56 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/21 15:00:13 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		description_helper(char *scene, int *len)
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

char			**get_description(char *scene, int i)
{
	char		*descr_buf;
	char		**description;
	int			len[2];
	int			start;

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

int				count_objects(int len, char *buf)
{
	int			i;
	int			obj_nmb;

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

void			split_box(int *scij, t_scene *scene, char *buf)
{
	if (buf[scij[2] + 1] == '[')
	{
		while (buf[scij[2]] != ']')
		{
			if (buf[scij[2]] == '{')
				scene->box_nmb++;
			scij[2]++;
			if (buf[scij[2]] == ']' && ft_isdigit(buf[scij[2] - 1]))
				scij[2]++;
		}
	}
	else
		scene->box_nmb++;
}

void			strequal(char *obn, int *scij, t_scene *scene, char *buf)
{
	if (ft_strequ(obn, "\t\"light\"") ||
			ft_strequ(obn, "{\n\t\"light\""))
		split_light(scij, scene, buf);
	if (ft_strequ(obn, "\t\"surface\"") ||
			ft_strequ(obn, "{\n\t\"surface\""))
		split_surface(scij, scene, buf);
	if (ft_strequ(obn, "\t\"box\"") ||
			ft_strequ(obn, "{\n\t\"box\""))
		split_box(scij, scene, buf);
	if (ft_strequ(obn, "\t\"camera\"") ||
			ft_strequ(obn, "{\n\t\"camera\""))
		scij[1]++;
}
