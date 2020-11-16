/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 13:38:13 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 18:20:28 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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
			scene->draw[scene->mode](sdl, scene);
		}
	}
	return (1);
}
