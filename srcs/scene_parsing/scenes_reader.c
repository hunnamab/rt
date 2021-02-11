/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:43 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/11 13:48:25 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	scene_objects(int *snmi, t_scene *scene, char *buf)
{
	char		*obj_name;
	char		**obj_desc;

	obj_name = ft_strsub(buf, snmi[0], (snmi[3] - snmi[0]));
	obj_desc = get_description(buf, snmi[3] + 2);
	if (!(ft_strequ(obj_name, "\t\"camera\":")) && \
		!(ft_strequ(obj_name, "\t\"light\":")) && snmi[1] < scene->obj_nmb)
	{
		get_parameters(obj_name, obj_desc, scene, snmi);
	}
	else if ((ft_strequ(obj_name, "\t\"camera\":")))
		get_camera(obj_desc, scene);
	else if ((ft_strequ(obj_name, "\t\"surface\":")) && snmi[4] < scene->srf_nmb)
		get_surface(obj_desc, scene, snmi);
	else if ((ft_strequ(obj_name, "\t\"light\":")) && snmi[2] < scene->light_nmb)
		get_light(obj_desc, scene, snmi);
	ft_memdel((void **)&obj_name);
	ft_memdel_float((void **)obj_desc);
	if (buf[snmi[3] + 1] == '{')
		while (buf[snmi[3]] != '}')
			snmi[3]++;
	else
		while (buf[snmi[3]] != ']')
		{
			snmi[3]++;
			if (buf[snmi[3]] == ']' && ft_isdigit(buf[snmi[3]- 1]))
				snmi[3]++;
		}
	snmi[0] = snmi[3] + 3;
}

static void	get_objects(char *buf, t_scene *scene, int len)
{
	int snmi[5];
	char c;
	int i = 0;
	int txt_nmb = 0;

	snmi[0] = 2;
	snmi[1] = 0;
	snmi[2] = 0;
	snmi[3] = 0;
	snmi[4] = 0;
	//if (!brackets(buf))
	//	output_error(6);
	scene->obj_nmb = count_objects(len, buf);
	split_objects(len, scene, buf);
	scene->srfs = protected_malloc(sizeof(t_cutting_surface), scene->srf_nmb);
	scene->objs = protected_malloc(sizeof(t_object *), scene->obj_nmb + 1);
	scene->light = protected_malloc(sizeof(t_light), scene->light_nmb);
	while (snmi[3] < len)
	{
		if (buf[snmi[3] + 1] == '{' || buf[snmi[3] + 1] == '[')
		{
			c = buf[snmi[3] + 1];
			scene_objects(snmi, scene, buf);
		}
		snmi[3]++;
	}
	while (i < scene->obj_nmb)
	{
		if (scene->objs[i]->text != NULL)
			txt_nmb++;
		if (scene->objs[i]->normal_text != NULL)
			txt_nmb++;
		i++;
	}
	scene->texts = protected_malloc(sizeof(t_texture *), txt_nmb + 1);
	int j = 0;
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
	ft_memdel((void **)&buf);
}

static void associate_obj_with_srf(t_scene *scene, int id, int i)
{
	int srf_one_obj = 0;
	int n = 0;
	int j = 0;
	while (n < scene->srf_nmb)
	{
		if (scene->srfs[n].object == id)
		{
			srf_one_obj++;
		}
		n++;
	}
	scene->objs[i]->cutting_surfaces = protected_malloc(sizeof(t_cutting_surface), srf_one_obj);
	n = 0;
	while (n < scene->srf_nmb)
	{
		if (scene->srfs[n].object == id)
		{
			ft_memcpy(&scene->objs[i]->cutting_surfaces[j], &scene->srfs[n], sizeof(t_cutting_surface));
			scene->objs[i]->cs_nmb++;
			j++;
		}
		n++;
	}
}

void		read_scene(int fd, t_scene *scene)
{
	int		ret;
	char	*buf;
	int i = 0;

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
