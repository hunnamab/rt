/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_deepth_buf.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 08:17:06 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/21 20:31:13 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void				all_frame_loop_deepth(t_sdl *sdl, t_scene *scene)
{
	int		xyi[3];
	t_color	color;

	xyi[1] = -1;
	while (++xyi[1] < HEI)
	{
		xyi[0] = -1;
		while (++xyi[0] < WID)
		{
			xyi[2] = xyi[1] * WID + xyi[0];
			color.red = scene->depth_buf[xyi[2]] > 255 ? \
			255 : scene->depth_buf[xyi[2]];
			color.green = color.red;
			color.blue = color.red;
			SDL_SetRenderDrawColor(sdl->renderer, color.red, color.green, \
														color.blue, 255);
			SDL_RenderDrawPoint(sdl->renderer, xyi[0], xyi[1]);
		}
	}
}

void					draw_deepth_buf(t_sdl *sdl, t_scene *scene)
{
	scene->bounce_cnt = 0;
	get_rays_arr(scene);
	get_closest_points(scene, 0, 0);
	clEnqueueReadBuffer(scene->cl_data.commands, \
		scene->cl_data.scene.depth_buf, CL_FALSE, 0, \
			sizeof(float) * WID * HEI, scene->depth_buf, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
	all_frame_loop_deepth(sdl, scene);
	draw_ui(sdl->renderer, scene->rt_ui);
	SDL_RenderPresent(sdl->renderer);
}
