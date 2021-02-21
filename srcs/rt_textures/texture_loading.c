/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loading.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:45:38 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/21 18:57:33 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		is_copy(t_scene *scene, int index) /*возвращает индекс текстуры в массиве текстур, если проверяемая текстура это копия*/
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
void		get_sizeof_unique_textures(t_scene *scene, cl_int2 *shift_buf, cl_int4 *index_buf, long long int *ijiss)
{
	ijiss[0] = -1;
	ijiss[1] = 0;
	ijiss[2] = 0;
	ijiss[3] = 0;
	ijiss[4] = 0;
	while (++ijiss[0] < scene->obj_nmb)
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
void	load_textures(t_scene *scene)
{
	long long unsigned int sizeoftextures = 0;
	int i;
	unsigned char *texture_buf = NULL;
	int shift = 0;
	int id = 0;
	i = 0;
	int j = 0;
	long long int ijiss[5]; /*худшее что я видел в жизни*/
	cl_int2 *shift_buf = malloc(sizeof(cl_int2) * scene->obj_nmb);
	cl_int4 *index_buf = malloc(sizeof(cl_int4) * scene->obj_nmb);
	ft_memset((void *)index_buf, -1, sizeof(cl_int4) * scene->obj_nmb);
	//get_sizeof_unique_textures(scene, shift_buf, index_buf, ijiss);
	//init_ijiss(ijiss);
	while (i < scene->obj_nmb)
	{
		if (scene->objs[i]->text && is_copy(scene, scene->objs[i]->texture_id) < 0)
		{
			sizeoftextures += scene->objs[i]->text->size;
			index_buf[i].s0 = i;
			index_buf[i].s1 = scene->objs[i]->texture_id;
		}
		else if (scene->objs[i]->text)
		{
			id = is_copy(scene, scene->objs[i]->texture_id);
			while(id != index_buf[j].s1)
				j++;
			index_buf[i].s0 = j;
			index_buf[i].s1 = -1;
			j = 0;
		}
		if (scene->objs[i]->normal_text && is_copy(scene, scene->objs[i]->normal_map_id) < 0)
		{
			sizeoftextures += scene->objs[i]->normal_text->size;
			index_buf[i].s3 = scene->objs[i]->normal_map_id;
			index_buf[i].s2 = i;
		}
		else if (scene->objs[i]->normal_text)
		{
			id = is_copy(scene, scene->objs[i]->normal_map_id);
			while (id != index_buf[j].s3)
				j++;
			index_buf[i].s2 = j;
			index_buf[i].s3 = -1;
			j = 0;
		}
		i++;
	}
	texture_buf = sizeoftextures > 0 ? malloc(sizeoftextures) : NULL;
	if (texture_buf != NULL)
	{
		ft_memset((void *)texture_buf, 0, sizeoftextures);
		i = -1;
		while (++i < scene->obj_nmb)
		{
			if (scene->objs[i]->text != NULL)
			{
				id = is_copy(scene, scene->objs[i]->texture_id);
				if (id < 0)
				{
					shift_buf[i].x = shift;
					scene->objs[i]->texture_id = shift_buf[i].x;
					shift += scene->objs[i]->text->size;
					ft_memcpy(texture_buf, scene->objs[i]->text->pixels, scene->objs[i]->text->size);
					texture_buf += scene->objs[i]->text->size;
				}
				else
				{
					while (id != index_buf[j].s1)
						j++;
					scene->objs[i]->texture_id = shift_buf[index_buf[j].s0].x;
					j = 0;
				}
			}
			if (scene->objs[i]->normal_text != NULL)
			{
				id = is_copy(scene, scene->objs[i]->normal_map_id);
				if (id < 0)
				{
					shift_buf[i].y = shift;
					scene->objs[i]->normal_map_id = shift_buf[i].y;
					shift += scene->objs[i]->normal_text->size;
					ft_memcpy(texture_buf, scene->objs[i]->normal_text->pixels, scene->objs[i]->normal_text->size);
					texture_buf += scene->objs[i]->normal_text->size;
				}
				else
				{
					while (id != index_buf[j].s3)
						j++;
					scene->objs[i]->normal_map_id = shift_buf[index_buf[j].s2].y;
					j = 0;
				}
			}
		}
		texture_buf -= shift;
		if (sizeoftextures > 0)
			scene->cl_data.scene.textures = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
				CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeoftextures, texture_buf, NULL);
		scene->obj_nmb > 0 ? free(shift_buf) : 0;
		scene->obj_nmb > 0 ? free(index_buf) : 0;
		sizeoftextures > 0 ? free(texture_buf) : 0;
	} 
}
