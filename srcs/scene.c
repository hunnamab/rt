/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:21:11 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/06 23:20:15 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_default(t_scene *scene)
{
	float	**matrix;
	int		x;

	x = -1;
	scene->viewport = protected_malloc(sizeof(cl_float3), (WID * HEI));
	scene->ray_buf = protected_malloc(sizeof(cl_float3), (WID * HEI));
	scene->normal_buf = protected_malloc(sizeof(cl_float3), WID * HEI);
	scene->material_buf = protected_malloc(sizeof(t_material), WID * HEI);
	scene->intersection_buf = protected_malloc(sizeof(cl_float3), WID * HEI);
	scene->index_buf = protected_malloc(sizeof(int), WID * HEI);
	scene->depth_buf = protected_malloc(sizeof(float), WID * HEI);	
	scene->frame_buf = protected_malloc(sizeof(t_color), (WID * HEI));
}

void	refresh_scene(t_scene *scene)
{
	//scene->draw[scene->mode](sdl, scene);
}

void	init_scene(t_scene *scene)
{
	scene->init[0] = &init_default;
	scene->init[1] = &init_default;
	scene->init[2] = &init_default;
	scene->init[3] = &init_default;
	scene->draw[0] = &draw_scene;
	scene->draw[1] = &draw_normal_buf;
	scene->draw[2] = &draw_deepth_buf;
	scene->draw[3] = &draw_raycast;
	scene->filter[0] = &sepia_filter;
	scene->filter[1] = &gauss_filter;
	scene->filter[2] = &gray_scale;
	scene->filter[NEGATIVE] = &negative;
	scene->filter[SHARPEN] = &sharpen_filter;
	scene->filter[MAGIC] = &magic_filter;
	scene->normal_buf = NULL;
	scene->material_buf = NULL;
	scene->intersection_buf = NULL;
	scene->ray_buf = NULL;
	scene->viewport = NULL;
	scene->index_buf = NULL;
	scene->depth_buf = NULL;
	scene->init[scene->mode](scene);
	scene->scrshot_nmb = 1;
}
