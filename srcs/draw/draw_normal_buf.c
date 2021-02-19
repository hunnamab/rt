/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_normal_buf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 08:15:46 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 09:02:12 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void				all_frame_loop_normal(t_sdl *sdl, t_scene *scene)
{
	t_color				color;
	int					xyi[3];

	xyi[1] = -1;
	while (++xyi[1] < HEI)
	{
		xyi[0] = -1;
		while (++xyi[0] < WID)
		{
			xyi[2] = xyi[1] * WID + xyi[0];
			if (scene->index_buf[xyi[2]] != -1)
			{
				color.red = (scene->normal_buf[xyi[2]].x * 0.5 + 0.5) * 100;
				color.green = (scene->normal_buf[xyi[2]].y * 0.5 + 0.5) * 100;
				color.blue = (scene->normal_buf[xyi[2]].z * 0.5 + 0.5) * 100;
			}
			else
				set_color_zero(&color);
			SDL_SetRenderDrawColor(sdl->renderer, \
			color.red, color.green, color.blue, 255);
			SDL_RenderDrawPoint(sdl->renderer, xyi[0], xyi[1]);
		}
	}
}

void					draw_normal_buf(t_sdl *sdl, t_scene *scene)
{
	scene->bounce_cnt = 0;
	scene->init_flag = 1;
	get_rays_arr(scene);
	get_closest_points(scene, 0, 0);
	get_intersection_buf(scene);
	get_normal_buf(scene);
	clEnqueueReadBuffer(scene->cl_data.commands, \
		scene->cl_data.scene.index_buf, CL_FALSE, 0, \
			sizeof(int) * (WID * HEI), scene->index_buf, 0, NULL, NULL);
	clEnqueueReadBuffer(scene->cl_data.commands, \
		scene->cl_data.scene.normal_buf, CL_FALSE, 0, \
			sizeof(cl_float3) * (WID * HEI), scene->normal_buf, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
	all_frame_loop_normal(sdl, scene);
	draw_ui(sdl->renderer, scene->rt_ui);
	SDL_RenderPresent(sdl->renderer);
}
