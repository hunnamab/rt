/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:43 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 14:47:38 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	scene_objects(int *snmi, t_scene *scene, char *buf)
{
	char		*obj_name;
	char		**obj_desc;

	obj_name = ft_strsub(buf, snmi[0], (snmi[3] - snmi[0])); // записываем название объекта
	obj_desc = get_description(buf, snmi[3] + 3); // записываем описание объекта
	if (!(ft_strequ(obj_name, "camera")) && \
		!(ft_strequ(obj_name, "light")) && snmi[1] < scene->obj_nmb)
	{
		scene->objs[snmi[1]] = get_parameters(obj_name, obj_desc); // создаем объект и получаем его характеристики
		snmi[1]++; // плюсуем индекс массива, если объект не камера
	}
	else if ((ft_strequ(obj_name, "camera")))
		scene->camera = get_camera(obj_desc);
	else if ((ft_strequ(obj_name, "light")) && snmi[2] < scene->light_nmb)
	{
		scene->light[snmi[2]] = get_light(obj_desc);
		snmi[2]++;
	}
	ft_memdel((void **)&obj_name); // освобождаем строки
	ft_memdel_double((void **)obj_desc);
	while (buf[snmi[3]] != '}') // переходим к описанию следующего объекта
		snmi[3]++;
	snmi[0] = snmi[3] + 3;
}

static void	get_objects(char *buf, t_scene *scene, int len)
{
	int snmi[4]; // start, n, m, i

	snmi[0] = 0;
	snmi[1] = 0;
	snmi[2] = 0;
	snmi[3] = 0;
	if (!brackets(buf))
		output_error(6);
	scene->obj_nmb = count_objects(len, buf); // выясняем кол-во объектов сцены
	split_objects(len, scene, buf);
	scene->objs = protected_malloc(sizeof(t_object *), scene->obj_nmb); // создаем массив структур для объектов
	scene->light = protected_malloc(sizeof(t_light *), scene->light_nmb);
	while (snmi[3] < len)
	{
		if (buf[snmi[3] + 1] == '{')
			scene_objects(snmi, scene, buf);
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
