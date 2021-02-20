/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 08:14:31 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/20 20:54:09 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			check_max_bounce(t_scene *scene)
{
	if (scene->max_bounces > 1)
	{
		if (scene->has_refraction && scene->bounce_cnt == 0)
		{
			get_fresnel_coeff(scene);
			get_refraction_ray(scene);
		}
		get_reflection_ray(scene);
	}
}

static void			bounce_loop(t_scene *scene, size_t global)
{
	get_closest_points(scene, 0, 0);
	scene->init_flag = 0;
	get_intersection_buf(scene);
	get_normal_buf(scene);
	get_material_buf(scene, 0);
	if (scene->bounce_cnt == 0)
		clEnqueueCopyBuffer(scene->cl_data.commands, \
scene->cl_data.scene.index_buf, scene->cl_data.scene.orig_index_buf,\
				0, 0, sizeof(int) * global, 0, NULL, NULL);
	clEnqueueCopyBuffer(scene->cl_data.commands, \
scene->cl_data.scene.material_buf, scene->cl_data.scene.prev_material_buf, \
			0, 0, sizeof(t_material) * global, 0, NULL, NULL);
	clEnqueueCopyBuffer(scene->cl_data.commands, \
scene->cl_data.scene.normal_buf, scene->cl_data.scene.copy_normal_buf, \
			0, 0, sizeof(cl_float3) * global, 0, NULL, NULL);
	clEnqueueCopyBuffer(scene->cl_data.commands, \
scene->cl_data.scene.intersection_buf, \
scene->cl_data.scene.copy_intersec_buf, 0, 0, \
				sizeof(cl_float3) * global, 0, NULL, NULL);
	get_frame_buf(scene, 0);
	check_max_bounce(scene);
}

static void			all_frame_loop(t_sdl *sdl, t_scene *scene)
{
	register int		x;
	register int		y;
	register int		i;
	int					j;

	x = -1;
	y = -1;
	i = 0;
	j = 0;
	while (++y < HEI)
	{
		while (++x < WID)
		{
			i = y * WID + x;
			SDL_SetRenderDrawColor(sdl->renderer, \
			scene->frame_buf[i].red, scene->frame_buf[i].green, \
						scene->frame_buf[i].blue, 255);
			SDL_RenderDrawPoint(sdl->renderer, x, y);
		}
		x = -1;
	}
	draw_ui(sdl->renderer, scene->rt_ui);
	SDL_RenderPresent(sdl->renderer);
}

void				draw_scene(t_sdl *sdl, t_scene *scene)
{
	size_t				global;

	global = WID * HEI;
	scene->bounce_cnt = 0;
	//scene->max_bounces = 3;
	scene->has_refraction = 0;
	scene->init_flag = 1;
	get_rays_arr(scene);
	while (scene->bounce_cnt < scene->max_bounces)
	{
		bounce_loop(scene, global);
		clFinish(scene->cl_data.commands);
		scene->bounce_cnt++;
	}
	if (scene->filter_type != DEFAULT)
		scene->filter[scene->filter_type](&scene->filter_data);
	clEnqueueReadBuffer(scene->cl_data.commands, \
	scene->cl_data.scene.frame_buf, 0, 0, sizeof(t_color) * global, \
						scene->frame_buf, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
	all_frame_loop(sdl, scene);
}
