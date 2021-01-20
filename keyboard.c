/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 13:38:13 by pmetron           #+#    #+#             */
/*   Updated: 2021/01/20 18:32:43 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	camera_left(t_scene *scene)
{
	scene->camera.rotation[1] -= 0.1;
	refresh_scene(scene);
}

void	camera_right(t_scene *scene)
{
	scene->camera.rotation[1] += 0.1;
	refresh_scene(scene);
}

void	camera_down(t_scene *scene)
{
	scene->camera.rotation[0] += 0.1;
	refresh_scene(scene);
}

void	camera_up(t_scene *scene)
{
	scene->camera.rotation[0] -= 0.1;
	refresh_scene(scene);
}

void	camera_forward(t_scene *scene)
{
	scene->camera.position.z -= 0.4;
	refresh_scene(scene);
}

void	camera_backward(t_scene *scene)
{
	scene->camera.position.z += 0.4;
	refresh_scene(scene);
}

void	camera_move_right(t_scene *scene)
{
	scene->camera.position.x -= 0.4;
	refresh_scene(scene);
}

void	camera_move_left(t_scene *scene)
{
	scene->camera.position.x += 0.4;
	refresh_scene(scene);
}

void	camera_move_up(t_scene *scene)
{
	scene->camera.position.y -= 0.4;
	refresh_scene(scene);
}

void	camera_move_down(t_scene *scene)
{
	scene->camera.position.y += 0.4;
	refresh_scene(scene);
}

uint8_t	*copy_pixels(t_scene *scene, uint8_t *pixels)
{
	int x;
	int y;
	int i;
	int j;

	y = -1;
	j = 0;
	pixels = (uint8_t *)malloc(WID * HEI * 4);
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			i = y * WID + x;
			pixels[j] = scene->pixels[i].blue;
			j++;
			pixels[j] = scene->pixels[i].green;
			j++;
			pixels[j] = scene->pixels[i].red;
			j++;
			pixels[j] = scene->pixels[i].alpha;
			j++;
		}
		i++;
	}
	return (pixels);
}

void	screen_png(t_scene *scene)
{
	SDL_Surface *srf;

	srf = SDL_CreateRGBSurface(0, WID , HEI, 32, 0, 0, 0, 0);
	srf->pixels = copy_pixels(scene, (uint8_t *)srf->pixels);
	IMG_SavePNG(srf, "screenshot.png");
	ft_putstr("Saved image in png\n");
	SDL_FreeSurface(srf);
}

int		keyboard(t_sdl *sdl, t_scene *scene)
{
	if (SDL_PollEvent(&sdl->event))
	{
		if (SDL_QUIT == sdl->event.type || (SDLK_ESCAPE == \
		sdl->event.key.keysym.sym && sdl->event.type == SDL_KEYDOWN))
			return (0);
		else if (sdl->event.type == SDL_KEYDOWN)
		{
			SDL_RenderClear(sdl->renderer);
			SDLK_RIGHT == sdl->event.key.keysym.sym ? camera_right(scene) : 0;
			SDLK_LEFT == sdl->event.key.keysym.sym ? camera_left(scene) : 0;
			SDLK_UP == sdl->event.key.keysym.sym ? camera_up(scene) : 0;
			SDLK_DOWN == sdl->event.key.keysym.sym ? camera_down(scene) : 0;
			SDLK_w ==  sdl->event.key.keysym.sym ? camera_forward(scene) : 0;
			SDLK_s ==  sdl->event.key.keysym.sym ? camera_backward(scene) : 0;
			SDLK_a ==  sdl->event.key.keysym.sym ? camera_move_left(scene) : 0;
			SDLK_d ==  sdl->event.key.keysym.sym ? camera_move_right(scene) : 0;
			SDLK_q ==  sdl->event.key.keysym.sym ? camera_move_up(scene) : 0;
			SDLK_e ==  sdl->event.key.keysym.sym ? camera_move_down(scene) : 0;
			SDLK_SPACE == sdl->event.key.keysym.sym ? screen_png(scene): 0;
		}
	}
	return (1);
}

