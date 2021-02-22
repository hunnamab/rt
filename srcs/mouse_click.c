/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_click.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 21:41:49 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/22 18:35:29 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	draw_filters_2(t_scene *scene, int i)
{
	if (i % 6 == 4)
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
}

void	draw_filters(t_scene *scene, int i)
{
	if (i % 6 == 1)
	{
		scene->filter_type = DEFAULT;
		scene->rt_ui->filt = 0;
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
	else
		draw_filters_2(scene, i);
}

void	click_2(t_sdl *sdl, t_scene *scene)
{
	if (check_rect(scene->rt_ui->def.rect, sdl->event))
		redraw_mode(scene, 4);
	if (check_rect(scene->rt_ui->left.rect, sdl->event))
	{
		scene->rt_ui->i--;
		scene->rt_ui->i = scene->rt_ui->i < 0 ? 6 : scene->rt_ui->i;
		draw_filters(scene, scene->rt_ui->i);
	}
	if (check_rect(scene->rt_ui->right.rect, sdl->event))
	{
		scene->rt_ui->i++;
		draw_filters(scene, scene->rt_ui->i);
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
	click_2(sdl, scene);
}
