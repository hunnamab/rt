#include "rt.h"

void    local_cutting(t_scene *scene)
{
    int i;
    int j;
    float **matrix;
    j = 0;
    i = 0; 
    while(i < scene->obj_nmb)
    {
        while(j < scene->objs[i]->cs_nmb)
        {
            if (scene->objs[i]->cutting_surfaces[j].is_local)
            {
                if(scene->objs[i]->cutting_surfaces[j].type == PLANE)
                {
                    scene->objs[i]->cutting_surfaces[j].param2 = \
                    vector_sub(&scene->objs[i]->position, &scene->objs[i]->cutting_surfaces[j].param2);
                    matrix = get_rotation_matrix(scene->objs[i]->rotation);
                    transform(&scene->objs[i]->cutting_surfaces[j].param1, matrix, 1);
                    matr_free(matrix, 4);
                    scene->objs[i]->cutting_surfaces[j].param3 = -(scene->objs[i]->cutting_surfaces[j].param1.x * scene->objs[i]->cutting_surfaces[j].param2.x) - scene->objs[i]->cutting_surfaces[j].param1.y * scene->objs[i]->cutting_surfaces[j].param2.y - scene->objs[i]->cutting_surfaces[j].param1.z * scene->objs[i]->cutting_surfaces[j].param2.z;
                }
                if(scene->objs[i]->cutting_surfaces[j].type == SPHERE)
                {
                    scene->objs[i]->cutting_surfaces[j].param1 = \
                    vector_sub(&scene->objs[i]->position, &scene->objs[i]->cutting_surfaces[j].param1);
                }
                if(scene->objs[i]->cutting_surfaces[j].type == CYLINDER)
                {
                    scene->objs[i]->cutting_surfaces[j].param1 = \
                    vector_sub(&scene->objs[i]->position, &scene->objs[i]->cutting_surfaces[j].param1);
                    scene->objs[i]->cutting_surfaces[j].param2.x += scene->objs[i]->rotation[0];
                    scene->objs[i]->cutting_surfaces[j].param2.y += scene->objs[i]->rotation[1];
                    scene->objs[i]->cutting_surfaces[j].param1.z += scene->objs[i]->rotation[2];
                }
            }
            j++;
        }
        j = 0;
        i++;
    }
}