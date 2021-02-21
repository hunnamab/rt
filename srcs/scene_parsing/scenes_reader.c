/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:43 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/21 14:48:16 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			do_scene_objects(char *buf, int *snmi)
{
	if (buf[snmi[3] + 1] == '{')
		while (buf[snmi[3]] != '}')
			snmi[3]++;
	else
		while (buf[snmi[3]] != ']')
		{
			snmi[3]++;
			if (buf[snmi[3]] == ']' && ft_isdigit(buf[snmi[3] - 1]))
				snmi[3]++;
		}
	snmi[0] = snmi[3] + 3;
}

void				scene_objects(int *snmi, t_scene *scene, char *buf)
{
	char			*obj_name;
	char			**obj_desc;

	obj_name = ft_strsub(buf, snmi[0], (snmi[3] - snmi[0]));
	obj_desc = get_description(buf, snmi[3] + 2);
	if (!(ft_strequ(obj_name, "\t\"camera\":")) && \
		!(ft_strequ(obj_name, "\t\"light\":")) && snmi[1] < scene->obj_nmb)
	{
		get_parameters(obj_name, obj_desc, scene, snmi);
	}
	else if ((ft_strequ(obj_name, "\t\"camera\":")))
		get_camera(obj_desc, scene);
	else if ((ft_strequ(obj_name, "\t\"surface\":"))
					&& snmi[4] < scene->srf_nmb)
		get_surface(obj_desc, scene, snmi);
	else if ((ft_strequ(obj_name, "\t\"light\":"))
					&& snmi[2] < scene->light_nmb)
		get_light(obj_desc, scene, snmi);
	else
		output_error(6);
	ft_memdel((void **)&obj_name);
	ft_memdel_float((void **)obj_desc);
	do_scene_objects(buf, snmi);
}

static int			count_srfs(t_scene *scene, int id)
{
	int				n;
	int				res;

	n = 0;
	res = 0;
	while (n < scene->srf_nmb)
	{
		if (scene->srfs[n].object == id)
			res++;
		n++;
	}
	return (res);
}

static void			associate_obj_with_srf(t_scene *scene, int id, int i)
{
	int				srf_one_obj;
	int				n;
	int				j;

	srf_one_obj = 0;
	n = 0;
	j = 0;
	srf_one_obj = count_srfs(scene, id);
	scene->objs[i]->cutting_surfaces =
			protected_malloc(sizeof(t_cutting_surface), srf_one_obj);
	n = 0;
	while (n < scene->srf_nmb)
	{
		if (scene->srfs[n].object == id)
		{
			ft_memcpy(&scene->objs[i]->cutting_surfaces[j],
					&scene->srfs[n], sizeof(t_cutting_surface));
			scene->objs[i]->cs_nmb++;
			j++;
		}
		n++;
	}
}

void				read_scene(int fd, t_scene *scene)
{
	int				ret;
	char			*buf;
	int				i;

	i = 0;
	buf = protected_malloc(sizeof(char), 256000);
	ret = read(fd, buf, 64000);
	if (ret < 0)
		output_error(3);
	buf[ret] = '\0';
	scene->obj_nmb = 0;
	scene->light_nmb = 0;
	scene->srf_nmb = 0;
	get_objects(buf, scene, ret);
	while (i < scene->obj_nmb)
	{
		if (scene->objs[i]->surface_id != 0)
			associate_obj_with_srf(scene, scene->objs[i]->surface_id, i);
		i++;
	}
}
