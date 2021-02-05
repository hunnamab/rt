#include "rt.h"

void	load_textures(t_scene *scene)
{
	long long unsigned int sizeoftextures = 0; //сумма массивов пикселей всех текстур
	int i;
	unsigned char *texture_buf = NULL;
	int shift = 0;
	int err = 0;
	i = 0;
	while(i < scene->obj_nmb)
	{
		if (scene->objs[i]->text != NULL) 
			sizeoftextures += scene->objs[i]->text->size;
		if (scene->objs[i]->normal_text != NULL)
			sizeoftextures += scene->texts[scene->objs[i]->normal_map_id]->size;
		i++;
	}
	texture_buf = sizeoftextures > 0 ? malloc(sizeoftextures) : NULL;
	if (texture_buf != NULL)
	{
		ft_memset((void *)texture_buf, 0, sizeoftextures);
		int current_size = 0;
		i = 0;
		int check = 0;
		while(i < scene->obj_nmb)
		{
			if (scene->objs[i]->text != NULL || scene->objs[i]->normal_text != NULL)
			{
				shift = scene->objs[i]->text->size;
				ft_memcpy(texture_buf, scene->objs[i]->text->pixels, shift);
				texture_buf += shift;
			}
			if (scene->objs[i]->normal_text != NULL)
			{
				shift = scene->objs[i]->text->size;
				ft_memcpy(texture_buf, scene->objs[i]->normal_text->pixels, shift);
				texture_buf += shift;
			}
			i++;
		}
		texture_buf -= sizeoftextures;
		if(sizeoftextures > 0)
			scene->cl_data.scene.textures = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
				CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeoftextures, texture_buf, &err);
	}
}
