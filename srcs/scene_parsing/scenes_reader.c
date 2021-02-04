/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scenes_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:43 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/04 21:34:23 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	choose_type(char *type)
{
	int lol = 0;

	if (ft_strequ(type, "\"plane\","))
		lol = PLANE;
	else if (ft_strequ(type, "\"sphere\","))
		lol = SPHERE;
	else if (ft_strequ(type, "\"cylinder\","))
		lol = CYLINDER;
	else
		output_error(5);
	return (lol);
}

t_cutting_surface		new_srf(cl_float3 *param, int *obj_neg, cl_float param3, char *type)
{
	t_cutting_surface new_srf;
	new_srf.type = choose_type(type);
	new_srf.object = obj_neg[0];
	new_srf.is_negative = obj_neg[1];
	new_srf.param1 = param[0];
	new_srf.param2 = param[1];
	new_srf.param3 = param3;
	if(new_srf.type == PLANE)
	{
		new_srf.param3 = -(new_srf.param1.x * new_srf.param2.x) - new_srf.param1.y * new_srf.param2.y - new_srf.param1.z * new_srf.param2.z;
		printf("new_srf d == %f\n", new_srf.param3);
	}
	return (new_srf);
}

static void	one_srf(char **description, t_scene *scene, int * snmi)
{
	t_cutting_surface		srf;
	cl_float3	param[2];
	cl_float	param3;
	int			obj_neg[2];
	char		*type;
	
	obj_neg[0] = atoi(get_coordinates(description[1]));
	type = get_light_type(description[2]); // printf("light type = |%s|\n", type);
	obj_neg[1] = atoi(get_coordinates(description[3]));
	param[0] = get_points(description[4]);
	param[1] = get_points(description[5]);
	param3 = ftoi(get_coordinates(description[6]));
	srf = new_srf(param, obj_neg, param3, type);
	scene->srfs[snmi[4]] = srf;
	snmi[4]++;
	free(type);
}

static t_cutting_surface		many_srfs(char **description, int *snmi, int i)
{
	t_cutting_surface		srf;
	cl_float3	param[2];
	cl_float	param3;
	int			obj_neg[2];
	char		*type;

	obj_neg[0] = atoi(get_coordinates(description[i + 1]));
	type = get_light_type(description[i + 2]);
	obj_neg[1] = atoi(get_coordinates(description[i + 3]));
	param[0] = get_points(description[i + 4]);
	param[1] = get_points(description[i + 5]);
	param3 = ftoi(get_coordinates(description[i + 6]));
	srf = new_srf(param, obj_neg, param3, type);
	free(type);
	return (srf);
}

static 	void	get_surface(char **description, t_scene *scene, int *snmi)
{
	t_cutting_surface		srf;
	//char		*type;
	int 		i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				//type = get_light_type(description[i + 1]);
				srf = many_srfs(description, snmi, i);
				scene->srfs[snmi[4]] = srf;
				snmi[4]++;
				i += 7;
				//free(type);
			}
		}
	}
	if (description[0][0] == '{')
		one_srf(description, scene, snmi);
}


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
	else if ((ft_strequ(obj_name, "\t\"surface\":")) && snmi[4] < scene->srf_nmb)
		get_surface(obj_desc, scene, snmi);
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
	int snmi[5]; // start, n, m, i
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
	scene->obj_nmb = count_objects(len, buf); // выясняем кол-во объектов сцены
	split_objects(len, scene, buf);
	scene->srfs = protected_malloc(sizeof(t_cutting_surface), scene->srf_nmb);
	scene->objs = protected_malloc(sizeof(t_object *), scene->obj_nmb + 1); // создаем массив структур для объектов
	scene->light = protected_malloc(sizeof(t_light), scene->light_nmb);
	//scene->texts = protected_malloc(sizeof(t_texture *), scene->obj_nmb * 2 + 1);
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
	/*i = 0;
	int j = 0;
	while (i < scene->obj_nmb)
	{
		if (i == scene->objs[i]->texture_id)
		{
			scene->texts[j] = scene->objs[i]->text;
			j++;
		}
		i++;
	}*/
	int j = 0;
	i = 0;
	printf("KNJFFKJNSVKLGNSLKGJANLKGNALK%d\n", txt_nmb);
	while (j < scene->obj_nmb)
	{
		if (scene->objs[j]->text != NULL)
		{
			scene->texts[i] = scene->objs[j]->text;
			printf("%d\n", scene->texts[i]->size);
			scene->objs[j]->texture_id = i;
			printf("%d\n", scene->objs[j]->texture_id);
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
			scene->objs[i]->cutting_surfaces[j] = scene->srfs[n];
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