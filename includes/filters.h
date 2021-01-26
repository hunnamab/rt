#ifndef FILTERS_H
# define FILTERS_H
#include "rt.h"
#include "types.h"

enum    filters
{
    SEPIA,
    GAUSS,
    GRAYCALE,
    NEGATIVE
};

void    filters_init(t_filter_data *data);
void    gauss_filter(t_filter_data data);
void    sepia_filter(t_filter_data *scene);
void    gray_scale(t_filter_data data);
void    negative(t_filter_data data);

#endif