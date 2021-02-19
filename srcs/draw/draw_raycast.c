/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_raycast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 00:18:09 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 08:57:09 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	all_frame_loop_raycast(t_sdl *sdl, t_scene *scene)
{
	int		x;
	int		y;
	t_color	color;
	int		i;

	x = -1;
	y = -1;
	i = 0;
	while (++y < HEI)
	{
		while (++x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
				color = scene->material_buf[i].color;
			else
				set_color_zero(&color);
			SDL_SetRenderDrawColor(sdl->renderer, \
			color.red, color.green, color.blue, 255);
			SDL_RenderDrawPoint(sdl->renderer, x, y);
		}
		x = -1;
	}
	draw_ui(sdl->renderer, scene->rt_ui);
	SDL_RenderPresent(sdl->renderer);
}

void		draw_raycast(t_sdl *sdl, t_scene *scene)
{
	scene->bounce_cnt = 0;
	get_rays_arr(scene);
	get_closest_points(scene, 0, 0);
	get_intersection_buf(scene);
	get_normal_buf(scene);
	get_material_buf(scene, 0);
	clEnqueueReadBuffer(scene->cl_data.commands, \
					scene->cl_data.scene.index_buf, CL_FALSE, 0, \
				sizeof(int) * (WID * HEI), scene->index_buf, 0, NULL, NULL);
	clEnqueueReadBuffer(scene->cl_data.commands, \
					scene->cl_data.scene.material_buf, CL_FALSE, 0, \
		sizeof(t_material) * (WID * HEI), scene->material_buf, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
	all_frame_loop_raycast(sdl, scene);
}
