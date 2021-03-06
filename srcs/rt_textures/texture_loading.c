/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loading.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:45:38 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 17:32:40 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int					is_copy(t_scene *scene, int index)
{
	int ij[2];

	ij[1] = 0;
	ij[0] = -1;
	while (++ij[0] < index)
	{
		if (scene->texts[ij[0]] && \
		scene->texts[ij[0]]->size == scene->texts[index]->size)
		{
			while (ij[1] < scene->texts[ij[0]]->size / 2)
			{
				if (scene->texts[ij[0]]->pixels[ij[1]] \
				== scene->texts[index]->pixels[ij[1]])
					ij[1]++;
				else
				{
					ij[1] = 0;
					break ;
				}
			}
			if (ij[1] > 0)
				return (ij[0]);
		}
	}
	return (-1);
}

void				init_ijiss(long long int *ijiss)
{
	ijiss[0] = -1;
	ijiss[1] = 0;
	ijiss[2] = 0;
	ijiss[3] = 0;
}

void				texture_part(t_scene *scene, \
cl_int4 *index_buf, long long int *ijiss)
{
	if (scene->objs[ijiss[0]]->text && \
	is_copy(scene, scene->objs[ijiss[0]]->texture_id) < 0)
	{
		ijiss[4] += scene->objs[ijiss[0]]->text->size;
		index_buf[ijiss[0]].s0 = ijiss[0];
		index_buf[ijiss[0]].s1 = scene->objs[ijiss[0]]->texture_id;
	}
	else if (scene->objs[ijiss[0]]->text)
	{
		ijiss[2] = is_copy(scene, scene->objs[ijiss[0]]->texture_id);
		while (ijiss[2] != index_buf[ijiss[1]].s1)
			ijiss[1]++;
		index_buf[ijiss[0]].s0 = ijiss[1];
		index_buf[ijiss[0]].s1 = -1;
		ijiss[1] = 0;
	}
}

void				get_sizeof_unique_textures(t_scene *scene, \
cl_int4 *index_buf, long long int *ijiss)
{
	init_ijiss(ijiss);
	ijiss[4] = 0;
	while (++ijiss[0] < scene->obj_nmb)
	{
		texture_part(scene, index_buf, ijiss);
		if (scene->objs[ijiss[0]]->normal_text && \
		is_copy(scene, scene->objs[ijiss[0]]->normal_map_id) < 0)
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

void				load_textures(t_scene *scene)
{
	unsigned char	*texture_buf;
	long long int	ijiss[5];
	cl_int4			*index_buf;

	scene->shift_buf = malloc(sizeof(cl_int2) * scene->obj_nmb);
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
			scene->cl_data.scene.textures = \
			clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
				CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, \
				ijiss[4], texture_buf, NULL);
		scene->obj_nmb > 0 ? free(scene->shift_buf) : 0;
		scene->obj_nmb > 0 ? free(index_buf) : 0;
		ijiss[4] > 0 ? free(texture_buf) : 0;
	}
}
