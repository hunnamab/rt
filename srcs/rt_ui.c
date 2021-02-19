/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_ui.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 21:36:21 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/19 07:39:04 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_butt			init_save_butt(SDL_Renderer *renderer, \
									SDL_Rect rect, char *text)
{
	t_butt		save;
	SDL_Surface *srf;

	save.rect = rect;
	save.x = (float)(rect.x + (float)rect.w / 2) / WID;
	save.y = (float)(rect.y + (float)rect.h / 2) / HEI;
	srf = IMG_Load(text);
	save.text = SDL_CreateTextureFromSurface(renderer, srf);
	SDL_FreeSurface(srf);
	return (save);
}

SDL_Rect		make_rect(int x, int y, int w, int h)
{
	SDL_Rect	rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return (rect);
}

void			init_ui_2(t_sdl *sdl, t_scene *scene)
{
	scene->rt_ui->left = init_save_butt(sdl->renderer,\
							make_rect(1355, 600, 45, 45), "rt_ui/left.png");
	scene->rt_ui->right = init_save_butt(sdl->renderer, \
							make_rect(1515, 600, 45, 45), "rt_ui/right.png");
	scene->rt_ui->filters = init_save_butt(sdl->renderer, \
						make_rect(1355, 520, 200, 50), "rt_ui/filters.png");
	scene->rt_ui->gauss = init_save_butt(sdl->renderer, \
							make_rect(1405, 600, 100, 40), "rt_ui/gauss.png");
	scene->rt_ui->sepia = init_save_butt(sdl->renderer, \
							make_rect(1410, 600, 100, 40), "rt_ui/sepia.png");
	scene->rt_ui->grey = init_save_butt(sdl->renderer, \
							make_rect(1410, 600, 100, 40), "rt_ui/grey.png");
	scene->rt_ui->fault = init_save_butt(sdl->renderer, \
						make_rect(1410, 600, 100, 40), "rt_ui/default.png");
	scene->rt_ui->negative = init_save_butt(sdl->renderer, \
						make_rect(1410, 600, 100, 40), "rt_ui/negative.png");
	scene->rt_ui->magic = init_save_butt(sdl->renderer, \
							make_rect(1410, 600, 100, 40), "rt_ui/magic.png");
	scene->rt_ui->sound = init_save_butt(sdl->renderer, \
							make_rect(1485, 10, 70, 70), "rt_ui/sound.png");
	scene->rt_ui->i = 7;
}

void			init_ui(t_sdl *sdl, t_scene *scene)
{
	scene->rt_ui = (t_ui *)protected_malloc(sizeof(t_ui), 1);
	scene->rt_ui->save_png = init_save_butt(sdl->renderer, \
							make_rect(1345, 10, 70, 70), "rt_ui/icon.png");
	scene->rt_ui->ambiance = init_save_butt(sdl->renderer, \
					make_rect(1355, 120, 200, 50), "rt_ui/ambiance.png");
	scene->rt_ui->am_plus = init_save_butt(sdl->renderer, \
						make_rect(1355, 200, 50, 50), "rt_ui/plus.png");
	scene->rt_ui->am_minus = init_save_butt(sdl->renderer, \
						make_rect(1505, 200, 50, 50), "rt_ui/minus.png");
	scene->rt_ui->modes = init_save_butt(sdl->renderer, \
						make_rect(1355, 300, 200, 60), "rt_ui/modes.png");
	scene->rt_ui->normal = init_save_butt(sdl->renderer, \
						make_rect(1355, 400, 40, 40), "rt_ui/1_icon.png");
	scene->rt_ui->deepth = init_save_butt(sdl->renderer, \
						make_rect(1408, 400, 40, 40), "rt_ui/2_icon.png");
	scene->rt_ui->raycast = init_save_butt(sdl->renderer, \
						make_rect(1461, 400, 40, 40), "rt_ui/3_icon.png");
	scene->rt_ui->def = init_save_butt(sdl->renderer, \
						make_rect(1513, 400, 40, 40), "rt_ui/4_icon.png");
	scene->rt_ui->back = init_save_butt(sdl->renderer, \
						make_rect(1280, 0, 350, 720), "rt_ui/black.png");
	init_ui_2(sdl, scene);
}
