/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 14:44:06 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 14:54:50 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

char		**get_description(char *scene, int i)
{
	char	*descr_buf;
	char	**description;
	int		len;
	int		start;

	len = 0;
	start = i;
	while (scene[i + 1] != '}')
	{
		i++;
		len++;
	}
	if (scene[i] != '\n')
		output_error(6);
	if (!(descr_buf = ft_strsub(scene, start, len)))
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
	return (obj_nmb);
}

static void	init_norme(int *norme)
{
	norme[0] = 0;
	norme[1] = 0;
	norme[2] = -1;
	norme[3] = 0;
}

void		split_objects(int len, t_scene *scene, char *buf)
{
	char	*obj_name;
	int		scij[4]; // start, camera, i, j

	init_norme(scij);
	while (++scij[2] < len)
	{
		if (buf[scij[2] + 1] == '{' && scij[3] < scene->obj_nmb)
		{
			if (!(obj_name = ft_strsub(buf, scij[0], (scij[2] - scij[0]))))
				output_error(6);
			if (ft_strequ(obj_name, "light"))
				scene->light_nmb++;
			if (ft_strequ(obj_name, "camera"))
				scij[1]++;
			ft_memdel((void **)&obj_name);
			while (buf[scij[2]] != '}')
				scij[2]++;
			scij[0] = scij[2] + 3;
			scij[3]++;
		}
	}
	scene->obj_nmb = scene->obj_nmb - scene->light_nmb - scij[1];
	if (scene->obj_nmb < 1 || scene->light_nmb < 1 || scij[1] != 1)
		output_error(0);
}
