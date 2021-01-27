/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 13:38:13 by pmetron           #+#    #+#             */
/*   Updated: 2021/01/27 22:53:55 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	camera_left(t_scene *scene)
{
	scene->camera.rotation[1] -= 0.1;
}

void	camera_right(t_scene *scene)
{
	scene->camera.rotation[1] += 0.1;
}

void	camera_down(t_scene *scene)
{
	scene->camera.rotation[0] += 0.1;
}

void	camera_up(t_scene *scene)
{
	scene->camera.rotation[0] -= 0.1;
}

void	camera_forward(t_scene *scene)
{
	scene->camera.position.z -= 0.4;
}

void	camera_backward(t_scene *scene)
{
	scene->camera.position.z += 0.4;
}

void	camera_move_right(t_scene *scene)
{
	scene->camera.position.x -= 0.4;
}

void	camera_move_left(t_scene *scene)
{
	scene->camera.position.x += 0.4;
}

void	camera_move_up(t_scene *scene)
{
	scene->camera.position.y -= 0.4;
}

void	camera_move_down(t_scene *scene)
{
	scene->camera.position.y += 0.4;
}

uint8_t	*copy_frame_buf(t_scene *scene, uint8_t *frame_buf)
{
	int x;
	int y;
	int i;
	int j;

	y = -1;
	j = 0;
	frame_buf = (uint8_t *)malloc(WID * HEI * 4);
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			i = y * WID + x;
			frame_buf[j] = scene->frame_buf[i].blue;
			j++;
			frame_buf[j] = scene->frame_buf[i].green;
			j++;
			frame_buf[j] = scene->frame_buf[i].red;
			j++;
			frame_buf[j] = scene->frame_buf[i].alpha;
			j++;
		}
		i++;
	}
	return (frame_buf);
}

void	screen_png(t_scene *scene)
{
	SDL_Surface *srf;

	srf = SDL_CreateRGBSurface(0, WID , HEI, 32, 0, 0, 0, 0);
	srf->pixels = copy_frame_buf(scene, (uint8_t *)srf->pixels);
	IMG_SavePNG(srf, "screenshot.png");
	ft_putstr("Saved image in png\n");
	free(srf->pixels);
	SDL_FreeSurface(srf);
}

int		check_rect(SDL_Rect rect, SDL_Event e)
{
	if ((e.button.x >= rect.x && e.button.x <= rect.x + rect.w) && (e.button.y >= rect.y && e.button.y <= rect.y + rect.h))
		return (1);
	return (0);
}

void	am_plus(t_scene *scene)
{
	int i = 0;

	while (i < scene->light_nmb && scene->light[i].intensity <= 1.0)
	{
		if (scene->light[i].type == AMBIENT)
			scene->light[i].intensity += 0.1;
		i++;		
	}
	refresh_scene(scene);
}

void	am_minus(t_scene *scene)
{
	int i = 0;

	while (i < scene->light_nmb && scene->light[i].intensity >= 0.1)
	{
		if (scene->light[i].type == AMBIENT)
			scene->light[i].intensity -= 0.1;
		i++;	
	}
	refresh_scene(scene);
}

void	redraw_mode(t_scene *scene, int mode)
{
	if (mode == 1)
		scene->mode = 1;
	if (mode == 2)
		scene->mode = 2;
	if (mode == 3)
		scene->mode = 3;
	if (mode == 4) 
		scene->mode = 0;
	refresh_scene(scene);
}

void	draw_filters(t_scene *scene, int i)
{
	if (i % 6 == 1)
	{
		scene->rt_ui->filt = 0;
		scene->filter_type = DEFAULT;
	}
	else if (i % 6 == 2)
	{
		scene->rt_ui->filt = 1;
		scene->filter_type = SEPIA;
	}
	else if (i % 6 == 3)
	{
		scene->rt_ui->filt = 2;
		scene->filter_type = GRAYSCALE;
	}
	else if (i % 6 == 4)
	{
		scene->rt_ui->filt = 3;
		scene->filter_type = GAUSS;
	}
	else if (i % 6 == 5)
	{
		scene->rt_ui->filt = 4;
		scene->filter_type = NEGATIVE;
	}
	else
	{
		scene->rt_ui->filt = 5;
		scene->filter_type = SHARPEN;
	}
	
	//scene->filter[scene->rt_ui->filt](scene);
	refresh_scene(scene);
}

void	click(t_sdl *sdl, t_scene *scene)
{
	if (check_rect(scene->rt_ui->save_png.rect, sdl->event))
		screen_png(scene);
	if (check_rect(scene->rt_ui->am_plus.rect, sdl->event))
		am_plus(scene);
	if (check_rect(scene->rt_ui->am_minus.rect, sdl->event))
		am_minus(scene);
	if (check_rect(scene->rt_ui->normal.rect, sdl->event))
		redraw_mode(scene, 1);
	if (check_rect(scene->rt_ui->deepth.rect, sdl->event))
		redraw_mode(scene, 2);
	if (check_rect(scene->rt_ui->raycast.rect, sdl->event))
		redraw_mode(scene, 3);
	if (check_rect(scene->rt_ui->def.rect, sdl->event))
		redraw_mode(scene, 4);
	if (check_rect(scene->rt_ui->left.rect, sdl->event))
	{
		scene->rt_ui->i--;
		draw_filters(scene, scene->rt_ui->i);
	}
	if (check_rect(scene->rt_ui->right.rect, sdl->event))
	{
		scene->rt_ui->i++;
		draw_filters(scene, scene->rt_ui->i);
	}
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
			scene->draw[scene->mode](sdl, scene);
			
		}
		else if (sdl->event.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_RenderClear(sdl->renderer);
			click(sdl, scene);
			clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.light, 0, 0, sizeof(t_light) * scene->light_nmb, scene->light, 0, NULL, NULL);
			scene->draw[scene->mode](sdl, scene);
		}
	}
	return (1);
}

