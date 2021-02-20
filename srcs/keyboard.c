/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 13:38:13 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/20 20:22:12 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

uint8_t			*copy_frame_buf(t_scene *scene, uint8_t *frame_buf)
{
	int			x;
	int			y;
	int			j;

	y = -1;
	j = 0;
	frame_buf = (uint8_t *)malloc(WID * HEI * 4);
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			frame_buf[j] = scene->frame_buf[y * WID + x].blue;
			j++;
			frame_buf[j] = scene->frame_buf[y * WID + x].green;
			j++;
			frame_buf[j] = scene->frame_buf[y * WID + x].red;
			j++;
			frame_buf[j] = scene->frame_buf[y * WID + x].alpha;
			j++;
		}
	}
	return (frame_buf);
}

static char		*get_name(int nmb)
{
	char		*tmp;

	tmp = ft_itoa(nmb);
	tmp = ft_strjoin(tmp, "_screenshot.png");
	if (!tmp)
		return (NULL);
	return (tmp);
}

void			screen_png(t_scene *scene)
{
	SDL_Surface	*srf;
	char		*name;

	srf = SDL_CreateRGBSurface(0, WID, HEI, 32, 0, 0, 0, 0);
	srf->pixels = copy_frame_buf(scene, (uint8_t *)srf->pixels);
	name = get_name(scene->scrshot_nmb);
	IMG_SavePNG(srf, name);
	ft_putstr("Saved image in png\n");
	scene->scrshot_nmb++;
	free(srf->pixels);
	free(name);
	SDL_FreeSurface(srf);
}

static void		key_down_event(t_sdl *sdl, t_scene *scene)
{
	SDL_RenderClear(sdl->renderer);
	SDLK_RIGHT == sdl->event.key.keysym.sym ? camera_right(scene) : 0;
	SDLK_LEFT == sdl->event.key.keysym.sym ? camera_left(scene) : 0;
	SDLK_UP == sdl->event.key.keysym.sym ? camera_up(scene) : 0;
	SDLK_DOWN == sdl->event.key.keysym.sym ? camera_down(scene) : 0;
	SDLK_w == sdl->event.key.keysym.sym ? camera_forward(scene) : 0;
	SDLK_s == sdl->event.key.keysym.sym ? camera_backward(scene) : 0;
	SDLK_a == sdl->event.key.keysym.sym ? camera_move_left(scene) : 0;
	SDLK_d == sdl->event.key.keysym.sym ? camera_move_right(scene) : 0;
	SDLK_q == sdl->event.key.keysym.sym ? camera_move_up(scene) : 0;
	SDLK_e == sdl->event.key.keysym.sym ? camera_move_down(scene) : 0;
	SDLK_SPACE == sdl->event.key.keysym.sym ? screen_png(scene) : 0;
	SDLK_m == sdl->event.key.keysym.sym ? scene->max_bounces++ : 0;
	if (scene->max_bounces > 1)
		SDLK_n == sdl->event.key.keysym.sym ? scene->max_bounces-- : 0;
	scene->draw[scene->mode](sdl, scene);
}

int				keyboard(t_sdl *sdl, t_scene *scene)
{
	if (SDL_PollEvent(&sdl->event))
	{
		if (SDL_QUIT == sdl->event.type || (SDLK_ESCAPE == \
		sdl->event.key.keysym.sym && sdl->event.type == SDL_KEYDOWN))
			return (0);
		else if (sdl->event.type == SDL_KEYDOWN)
			key_down_event(sdl, scene);
		else if (sdl->event.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_RenderClear(sdl->renderer);
			click(sdl, scene);
			clEnqueueWriteBuffer(scene->cl_data.commands, \
								scene->cl_data.scene.light, 0, 0, \
			sizeof(t_light) * scene->light_nmb, scene->light, 0, NULL, NULL);
			scene->draw[scene->mode](sdl, scene);
		}
	}
	return (1);
}
