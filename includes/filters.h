#ifndef FILTERS_H
# define FILTERS_H
#include "rt.h"
#include "types.h"

void    filters_init(t_filter_data filter_data);
void    gauss_filter(t_scene *scene);
void    sepia_filter(t_scene *scene);
void    gray_scale(t_scene *scene);

#endif