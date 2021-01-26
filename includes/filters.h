#ifndef FILTERS_H
# define FILTERS_H
#include "rt.h"
#include "types.h"

enum    filters
{
    GAUSS,
    SEPIA,
    GRAYCALE,
    NEGATIVE
};

void    filters_init(t_filter_data filter_data);
void    gauss_filter(t_scene *scene);
void    sepia_filter(t_scene *scene);
void    gray_scale(t_scene *scene);
void    negative(t_filter_data data);

#endif