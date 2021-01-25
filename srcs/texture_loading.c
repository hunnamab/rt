#include "rt.h"

void    load_textures(t_scene *scene)
{
    long long unsigned int sizeoftextures = 0; //сумма массивов пикселей всех текстур
    int i;
    unsigned char *texture_buf;
    int shift = 0;
    int err = 0;
    i = 0;
    while(i < scene->obj_nmb)
    {
        if (scene->objs[i]->text != NULL)
            sizeoftextures += scene->objs[i]->text->size;
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
            if (scene->objs[i]->text != NULL)
            {
                shift = scene->objs[i]->text->size;
                ft_memcpy(texture_buf, scene->objs[i]->text->pixels, shift);
                texture_buf += shift;
            }
            i++;
        }
        current_size = 0;
        texture_buf -= sizeoftextures;
        for (int k = 0; k < scene->obj_nmb; k++)
        {
            if (scene->objs[k]->text != NULL)
                current_size += scene->objs[k]->text->size;
        }
        printf("sizeoftext %llu\n", sizeoftextures);
        scene->cl_data.scene.textures = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
            CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeoftextures, texture_buf, &err);
        int index = 1280 * 360 - 640;
        printf("texture (%hhu,%hhu,%hhu,%hhu)\n", texture_buf[index], texture_buf[index + 1], texture_buf[index + 2], texture_buf[index + 3]);
        printf("pixels (%hhu,%hhu,%hhu,%hhu)\n", scene->texts[0]->pixels[index], scene->texts[0]->pixels[index + 1], scene->texts[0]->pixels[index + 2], scene->texts[0]->pixels[index + 3]);
        if (err == 0)
            printf("textures copied on the device\n");
    }
}