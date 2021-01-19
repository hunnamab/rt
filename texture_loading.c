#include "rt.h"

void    load_textures(t_scene *scene)
{
    long long unsigned int sizeoftextures; //сумма массивов пикселей всех текстур
    int i;
    char *texture_buf;
    int shift;
    int err;
    i = 0;
    while(i < scene->obj_nmb)
    {
        if (scene->objs[i]->text != NULL)
            sizeoftextures += scene->objs[i]->text->size;
        i++;
    }
    texture_buf = malloc(sizeoftextures);
    ft_memset((void *)texture_buf, 0, sizeoftextures);
    int current_size = 0;
    i = 0;
    while(i < scene->obj_nmb)
    {
        if (scene->objs[i]->text != NULL)
        {
            shift = scene->objs[i]->text->size;
            ft_memcpy(texture_buf, scene->objs[i]->text->pixels, shift);
            texture_buf += shift;
        }
        i++;
    }
    texture_buf -= sizeoftextures;
    current_size = 0;
    for (int k = 0; k < scene->obj_nmb; k++)
    {
         if (scene->objs[k]->text != NULL)
         {
            for(int l = 0; l < scene->objs[k]->text->size; l++)
                if(texture_buf[current_size + l] != scene->objs[k]->text->pixels[l])
                    printf("copy texture error text_buf == %d pixel == %d\n", texture_buf[current_size + l], scene->objs[k]->text->pixels[l]);
            current_size += scene->objs[k]->text->size;
         }
    }
    printf("sizeoftext %llu\n", sizeoftextures);
    clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeoftextures, texture_buf, &err);
    if (err == 0)
        printf("textures copied on the device\n");
}