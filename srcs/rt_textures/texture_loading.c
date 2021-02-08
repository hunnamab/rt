#include "rt.h"

int		is_copy(t_scene *scene, int index) /*возвращает индекс текстуры в массиве текстур, если проверяемая текстура это копия*/
{
	int i;
	int j;

	j = 0;
	i = 0;
	while(i < index)
	{
		if (scene->texts[i] && scene->texts[i]->size == scene->texts[index]->size)
		{
			while(j < scene->texts[i]->size / 2)
			{
				if (scene->texts[i]->pixels[j] == scene->texts[index]->pixels[j])
					j++;
				else
				{
					j = 0;
					break;
				}
			}
			if(j > 0)
			{
				return(i);
			}
		}
		i++;
	}
	return(-1);
}

void	load_textures(t_scene *scene)
{
	long long unsigned int sizeoftextures = 0; //сумма массивов пикселей всех текстур
	int i;
	unsigned char *texture_buf = NULL;
	int shift = 0;
	int err = 0;
	int id = 0;
	i = 0;
	int j = 0;
	cl_int2 *shift_buf = malloc(sizeof(cl_int2) * scene->obj_nmb);
	cl_int4 *index_buf = malloc(sizeof(cl_int4) * scene->obj_nmb);
	ft_memset((void *)index_buf, -1, sizeof(cl_int4) * scene->obj_nmb);
	while(i < scene->obj_nmb)
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
		i = 0;
		while(i < scene->obj_nmb)
		{
			if (scene->objs[i]->text != NULL)
			{
				id = is_copy(scene, scene->objs[i]->texture_id);
				if(id < 0)
				{
					shift_buf[i].x = shift;
					scene->objs[i]->texture_id = shift_buf[i].x;
					shift += scene->objs[i]->text->size;
					ft_memcpy(texture_buf, scene->objs[i]->text->pixels, scene->objs[i]->text->size);
					texture_buf += scene->objs[i]->text->size;
				}
				else
				{
					while(id != index_buf[j].s1)
						j++;
					scene->objs[i]->texture_id = shift_buf[index_buf[j].s0].x;
					j = 0;
				}
			}
			if (scene->objs[i]->normal_text != NULL)
			{
				id = is_copy(scene, scene->objs[i]->normal_map_id);
				if(id < 0)
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
			i++;
		}
		texture_buf -= shift;
		if(sizeoftextures > 0)
			scene->cl_data.scene.textures = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
				CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeoftextures, texture_buf, &err);
		printf("загрузка текстур в видеопамять %d\n", err);
	} 
}
