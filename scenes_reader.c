/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:43 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/12 18:35:23 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	scene_objects(int *snmi, t_scene *scene, char *buf)
{
	char		*obj_name;
	char		**obj_desc;

	obj_name = ft_strsub(buf, snmi[0], (snmi[3] - snmi[0])); // записываем название объекта
	obj_desc = get_description(buf, snmi[3] + 2); // записываем описание объекта
	if (!(ft_strequ(obj_name, "\t\"camera\":")) && \
		!(ft_strequ(obj_name, "\t\"light\":")) && snmi[1] < scene->obj_nmb)
	{
		get_parameters(obj_name, obj_desc, scene, snmi); // создаем объект и получаем его характеристики
	}
	else if ((ft_strequ(obj_name, "\t\"camera\":")))
		get_camera(obj_desc, scene);
	else if ((ft_strequ(obj_name, "\t\"light\":")) && snmi[2] < scene->light_nmb)
		get_light(obj_desc, scene, snmi);
	ft_memdel((void **)&obj_name); // освобождаем строки
	ft_memdel_float((void **)obj_desc);
	if (buf[snmi[3] + 1] == '{')
		while (buf[snmi[3]] != '}') // переходим к описанию следующего объекта
			snmi[3]++;
	else
		while (buf[snmi[3]] != ']')
		{// переходим к описанию следующего объекта
			snmi[3]++;
			if (buf[snmi[3]] == ']' && ft_isdigit(buf[snmi[3]- 1]))
				snmi[3]++;
		}
	snmi[0] = snmi[3] + 3;
}

static void	get_objects(char *buf, t_scene *scene, int len)
{
	int snmi[4]; // start, n, m, i
	char c;

	snmi[0] = 2;
	snmi[1] = 0;
	snmi[2] = 0;
	snmi[3] = 0;
	//if (!brackets(buf))
	//	output_error(6);
	scene->obj_nmb = count_objects(len, buf); // выясняем кол-во объектов сцены
	split_objects(len, scene, buf);
	scene->objs = protected_malloc(sizeof(t_object *), scene->obj_nmb); // создаем массив структур для объектов
	scene->light = protected_malloc(sizeof(t_light *), scene->light_nmb);
	while (snmi[3] < len)
	{
		if (buf[snmi[3] + 1] == '{' || buf[snmi[3] + 1] == '[')
		{
			c = buf[snmi[3] + 1];
			scene_objects(snmi, scene, buf);
		}
		/*if (c == '{')
		{
			while (buf[snmi[3] + 1] != '}')
				snmi[3]++;
		}
		else if (c =='[')
		{
			while (buf[snmi[3] + 1] != ']')
				snmi[3]++;
		}*/
		snmi[3]++;
	}
	ft_memdel((void **)&buf);
}

void		read_scene(int fd, t_scene *scene)
{
	int		ret;
	char	*buf;

	buf = protected_malloc(sizeof(char), 256000);
	ret = read(fd, buf, 64000);
	if (ret < 0)
		output_error(3);
	buf[ret] = '\0';
	scene->obj_nmb = 0;
	scene->light_nmb = 0;
	get_objects(buf, scene, ret);
}
