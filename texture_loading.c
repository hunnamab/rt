#include "rt.h"

void    load_textures(t_scene *scene)
{
    long long unsigned int sizeoftextures; //сумма массивов пикселей всех текстур
    int i;

    i = 0;
    while(i < scene->obj_nmb)
    {
        if (scene->objs[i]->text != NULL)
            sizeoftextures += scene->objs[i]->text->size;
        i++;
    }
    printf("sizeoftext %llu\n", sizeoftextures);
}