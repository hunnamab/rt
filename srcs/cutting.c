#include "rt.h"

void    local_cutting(t_scene *scene)
{
    int i;
    int j;
    float **matrix;
    j = 0;
    i = 0; 
   // scene->objs[0]->cutting_surfaces[0].is_local = 1;
    while(i < scene->obj_nmb)
    {
        while(j < scene->objs[i]->cs_nmb)
        {
            if (scene->objs[i]->cutting_surfaces[j].is_local)
            {
                if(scene->objs[i]->cutting_surfaces[j].type == PLANE)
                {
                    printf("\n\nlul\n\n");
                    scene->objs[i]->cutting_surfaces[j].param2 = \
                    vector_sub(&scene->objs[i]->position, &scene->objs[i]->cutting_surfaces[j].param2);
                    matrix = get_rotation_matrix(scene->objs[i]->rotation);
                    printf("param2 (%f,%f,%f)\n", scene->objs[i]->cutting_surfaces[j].param2.x, scene->objs[i]->cutting_surfaces[j].param2.y, scene->objs[i]->cutting_surfaces[j].param2.z);
                    transform(&scene->objs[i]->cutting_surfaces[j].param1, matrix, 1);
                    matr_free(matrix, 4);
                    scene->objs[i]->cutting_surfaces[j].param3 = -(scene->objs[i]->cutting_surfaces[j].param1.x * scene->objs[i]->cutting_surfaces[j].param2.x) - scene->objs[i]->cutting_surfaces[j].param1.y * scene->objs[i]->cutting_surfaces[j].param2.y - scene->objs[i]->cutting_surfaces[j].param1.z * scene->objs[i]->cutting_surfaces[j].param2.z;
                }
                if(scene->objs[i]->cutting_surfaces[j].type == SPHERE)
                {
                    scene->objs[i]->cutting_surfaces[j].param2 = \
                    vector_sub(&scene->objs[i]->position, &scene->objs[i]->cutting_surfaces[j].param2);
                }
            }
            j++;
        }
        i++;
    }
}