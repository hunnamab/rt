/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loading.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:45:38 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/21 20:46:00 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		is_copy(t_scene *scene, int index)
{
	int i;
	int j;

	j = 0;
	i = -1;
	while (++i < index)
	{
		if (scene->texts[i] && scene->texts[i]->size == scene->texts[index]->size)
		{
			while (j < scene->texts[i]->size / 2)
			{
				if (scene->texts[i]->pixels[j] == scene->texts[index]->pixels[j])
					j++;
				else
				{
					j = 0;
					break;
				}
			}
			if (j > 0)
				return (i);
		}
	}
	return (-1);
}
void		init_ijiss(long long int *ijiss)
{
	ijiss[0] = -1;
	ijiss[1] = 0;
	ijiss[2] = 0;
	ijiss[3] = 0;
}
/*памятка борца с нормой: ijiss[0] == i, ijiss[1] == j,*/
/*ijiss[2] == id, ijiss[3] == shift, ijiss[4] == sizeoftextures*/
void				texture_part(t_scene *scene, cl_int4 *index_buf, long long int *ijiss)
{
	if (scene->objs[ijiss[0]]->text && is_copy(scene, scene->objs[ijiss[0]]->texture_id) < 0)
	{
		ijiss[4] += scene->objs[ijiss[0]]->text->size;
		index_buf[ijiss[0]].s0 = ijiss[0];
		index_buf[ijiss[0]].s1 = scene->objs[ijiss[0]]->texture_id;
	}
	else if (scene->objs[ijiss[0]]->text)
	{
		ijiss[2] = is_copy(scene, scene->objs[ijiss[0]]->texture_id);
		while(ijiss[2] != index_buf[ijiss[1]].s1)
			ijiss[1]++;
		index_buf[ijiss[0]].s0 = ijiss[1];
		index_buf[ijiss[0]].s1 = -1;
		ijiss[1] = 0;
	}
}

void				get_sizeof_unique_textures(t_scene *scene, cl_int4 *index_buf, long long int *ijiss)
{
	init_ijiss(ijiss);
	ijiss[4] = 0;
	while (++ijiss[0] < scene->obj_nmb)
	{
		texture_part(scene, index_buf, ijiss);
		if (scene->objs[ijiss[0]]->normal_text && is_copy(scene, scene->objs[ijiss[0]]->normal_map_id) < 0)
		{
			ijiss[4] += scene->objs[ijiss[0]]->normal_text->size;
			index_buf[ijiss[0]].s3 = scene->objs[ijiss[0]]->normal_map_id;
			index_buf[ijiss[0]].s2 = ijiss[0];
		}
		else if (scene->objs[ijiss[0]]->normal_text)
		{
			ijiss[2] = is_copy(scene, scene->objs[ijiss[0]]->normal_map_id);
			while (ijiss[2] != index_buf[ijiss[1]].s3)
				ijiss[1]++;
			index_buf[ijiss[0]].s2 = ijiss[1];
			index_buf[ijiss[0]].s3 = -1;
			ijiss[1] = 0;
		}
	}
}

static void 		norme(t_scene *scene, cl_int4 *index_buf, long long int *ijiss, unsigned char *texture_buf)
{
	scene->shift_buf[ijiss[0]].x = ijiss[3];
	scene->objs[ijiss[0]]->texture_id = scene->shift_buf[ijiss[0]].x;
	ijiss[3] += scene->objs[ijiss[0]]->text->size;
	ft_memcpy(texture_buf, scene->objs[ijiss[0]]->text->pixels, \
	scene->objs[ijiss[0]]->text->size);
	texture_buf += scene->objs[ijiss[0]]->text->size;
}

static void 		norme_second(t_scene *scene, cl_int4 *index_buf, long long int *ijiss)
{
	while (ijiss[2] != index_buf[ijiss[1]].s1)
		ijiss[1]++;
	scene->objs[ijiss[0]]->texture_id = scene->shift_buf[index_buf[ijiss[1]].s0].x;
	ijiss[1] = 0;
}

static	void		norme_third(t_scene *scene, cl_int4 *index_buf, long long int *ijiss, unsigned char *texture_buf)
{
	scene->shift_buf[ijiss[0]].y = ijiss[3];
	scene->objs[ijiss[0]]->normal_map_id = scene->shift_buf[ijiss[0]].y;
	ijiss[3] += scene->objs[ijiss[0]]->normal_text->size;
	ft_memcpy(texture_buf, scene->objs[ijiss[0]]->\
		normal_text->pixels, scene->objs[ijiss[0]]->normal_text->size);
}

static	void		norme_fourth(t_scene *scene, cl_int4 *index_buf, long long int *ijiss)
{
	while (ijiss[2] != index_buf[ijiss[1]].s3)
		ijiss[1]++;
	scene->objs[ijiss[0]]->normal_map_id = scene->shift_buf[index_buf[ijiss[1]].s2].y;
	ijiss[1] = 0;
}

void				get_texture_buf(t_scene *scene, cl_int4 *index_buf, \
unsigned char *texture_buf, long long int *ijiss)
{
	while (++ijiss[0] < scene->obj_nmb)
	{
		if (scene->objs[ijiss[0]]->text != NULL)
		{
			if ((ijiss[2] = is_copy(scene, \
			scene->objs[ijiss[0]]->texture_id)) < 0)
			{
				norme(scene, index_buf, ijiss, texture_buf);
				texture_buf += scene->objs[ijiss[0]]->text->size;
			}
			else
				norme_second(scene, index_buf, ijiss);
		}
		if (scene->objs[ijiss[0]]->normal_text != NULL)
		{
			if ((ijiss[2] = is_copy(scene, \
			scene->objs[ijiss[0]]->normal_map_id)) < 0)
			{
				norme_third(scene, index_buf, ijiss, texture_buf);
				texture_buf += scene->objs[ijiss[0]]->normal_text->size;
			}
			else
				norme_fourth(scene, index_buf, ijiss);
		}
	}
}

void				load_textures(t_scene *scene)
{
	unsigned char *texture_buf;
	long long int ijiss[5]; /*худшее что я видел в жизни*/
	scene->shift_buf = malloc(sizeof(cl_int2) * scene->obj_nmb);
	cl_int4 *index_buf;
	
	index_buf = malloc(sizeof(cl_int4) * scene->obj_nmb);
	texture_buf = NULL;
	ft_memset((void *)index_buf, -1, sizeof(cl_int4) * scene->obj_nmb);
	get_sizeof_unique_textures(scene, index_buf, ijiss);
	init_ijiss(ijiss);
	texture_buf = ijiss[4] > 0 ? malloc(ijiss[4]) : NULL;
	if (texture_buf != NULL)
	{
		get_texture_buf(scene, index_buf, texture_buf, ijiss);
		if (ijiss[4] > 0)
			scene->cl_data.scene.textures = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
				CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, ijiss[4], texture_buf, NULL);
		scene->obj_nmb > 0 ? free(scene->shift_buf) : 0;
		scene->obj_nmb > 0 ? free(index_buf) : 0;
		ijiss[4] > 0 ? free(texture_buf) : 0;
	}
}
