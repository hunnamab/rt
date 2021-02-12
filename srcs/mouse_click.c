/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_click.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 21:41:49 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/12 21:44:55 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
	//refresh_scene(scene);
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
	//refresh_scene(scene);
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
	//refresh_scene(scene);
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
		scene->filter_type = MAGIC;
	}
	//scene->filter[scene->rt_ui->filt](scene);
	//refresh_scene(scene);
}
void	music_control(t_scene *scene)
{
	if (scene->m_flag == 0)
	{
		scene->m_flag = 1;
		Mix_ResumeMusic();
		return ;
	}
	else
	{
		scene->m_flag = 0;
		Mix_PauseMusic();
	}
}

void	click(t_sdl *sdl, t_scene *scene)
{
	if (check_rect(scene->rt_ui->save_png.rect, sdl->event))
		screen_png(scene);
	if (check_rect(scene->rt_ui->sound.rect, sdl->event))
		music_control(scene);
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