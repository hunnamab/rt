/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:34:50 by pmetron           #+#    #+#             */
/*   Updated: 2021/01/19 17:57:33 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	draw_scene(t_sdl *sdl, t_scene *scene)
{
	int		x;
	int		y;
	t_color	color;
	int		i;

	x = -1;
	y = -1;
	i = 0;

	size_t global = WID * HEI;
	size_t local;

	printf("sizeof light (host) = %lu\n", sizeof(t_light));
	
	clSetKernelArg(scene->cl_data.kernels[9], 0, sizeof(cl_mem), &scene->cl_data.scene.frame_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 1, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 2, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 3, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 5, sizeof(cl_mem), &scene->cl_data.scene.material_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 6, sizeof(cl_mem), &scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[9], 7, sizeof(cl_mem), &scene->cl_data.scene.light);
	clSetKernelArg(scene->cl_data.kernels[9], 8, sizeof(int), (void *)&scene->light_nmb);

	clGetKernelWorkGroupInfo(scene->cl_data.kernels[9], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[9], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
	//clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.intersection_buf, CL_TRUE, 0, sizeof(cl_float3) * global, scene->intersection_buf, 0, NULL, NULL);
	scene->pixels = protected_malloc(sizeof(t_color), (WID * HEI));
	while (++y < HEI)
	{
		while (++x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				color = reflection_color(scene, i);
				copy_color(&scene->pixels[i],  &color);
			}
			else
				set_color_zero(&color);
			SDL_SetRenderDrawColor(sdl->renderer, \
			color.red, color.green, color.blue, 255);
			SDL_RenderDrawPoint(sdl->renderer, x, y);
		}
		x = -1;
	}
	SDL_RenderPresent(sdl->renderer);
}

void	draw_normal_buf(t_sdl *sdl, t_scene *scene)
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
	SDL_RenderPresent(sdl->renderer);
}

void	draw_deepth_buf(t_sdl *sdl, t_scene *scene)
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
			if (scene->index_buf[xyi[2]] != -1)
			{
				color.red = scene->depth_buf[xyi[2]] > 255 ? \
				255 : scene->depth_buf[xyi[2]];
				color.green = color.red;
				color.blue = color.red;
			}
			else
				set_color_zero(&color);
			SDL_SetRenderDrawColor(sdl->renderer, \
			color.red, color.green, color.blue, 255);
			SDL_RenderDrawPoint(sdl->renderer, xyi[0], xyi[1]);
		}
	}
	SDL_RenderPresent(sdl->renderer);
}

void	draw_raycast(t_sdl *sdl, t_scene *scene)
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
	SDL_RenderPresent(sdl->renderer);
}
