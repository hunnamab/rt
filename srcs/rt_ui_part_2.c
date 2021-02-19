/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_ui_part_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 07:32:29 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 07:40:00 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			init_music(void)
{
	Mix_Music	*mus;
	int			result;
	int			flags;

	result = 0;
	flags = MIX_INIT_OGG;
	if (flags != (result = Mix_Init(flags)))
		exit(1);
	Mix_OpenAudio(22050, AUDIO_S16SYS, 4, 640);
	mus = Mix_LoadMUS("lights.wav");
	Mix_PlayMusic(mus, -1);
}

void			draw_ui_continue(SDL_Renderer *renderer, t_ui *rt_ui)
{
	if (rt_ui->filt == 3)
		SDL_RenderCopy(renderer, rt_ui->gauss.text, NULL, &rt_ui->gauss.rect);
	if (rt_ui->filt == 1)
		SDL_RenderCopy(renderer, rt_ui->sepia.text, NULL, &rt_ui->sepia.rect);
	if (rt_ui->filt == 2)
		SDL_RenderCopy(renderer, rt_ui->grey.text, NULL, &rt_ui->grey.rect);
	if (rt_ui->filt == 0)
		SDL_RenderCopy(renderer, rt_ui->fault.text, NULL, &rt_ui->fault.rect);
	if (rt_ui->filt == 4)
		SDL_RenderCopy(renderer, rt_ui->negative.text, NULL, \
													&rt_ui->negative.rect);
	if (rt_ui->filt == 5)
		SDL_RenderCopy(renderer, rt_ui->magic.text, NULL, &rt_ui->magic.rect);
}

void			draw_ui(SDL_Renderer *renderer, t_ui *rt_ui)
{
	SDL_RenderCopy(renderer, rt_ui->back.text, NULL, &rt_ui->back.rect);
	SDL_RenderCopy(renderer, rt_ui->save_png.text, NULL, &rt_ui->save_png.rect);
	SDL_RenderCopy(renderer, rt_ui->ambiance.text, NULL, &rt_ui->ambiance.rect);
	SDL_RenderCopy(renderer, rt_ui->am_plus.text, NULL, &rt_ui->am_plus.rect);
	SDL_RenderCopy(renderer, rt_ui->am_minus.text, NULL, &rt_ui->am_minus.rect);
	SDL_RenderCopy(renderer, rt_ui->modes.text, NULL, &rt_ui->modes.rect);
	SDL_RenderCopy(renderer, rt_ui->normal.text, NULL, &rt_ui->normal.rect);
	SDL_RenderCopy(renderer, rt_ui->deepth.text, NULL, &rt_ui->deepth.rect);
	SDL_RenderCopy(renderer, rt_ui->raycast.text, NULL, &rt_ui->raycast.rect);
	SDL_RenderCopy(renderer, rt_ui->def.text, NULL, &rt_ui->def.rect);
	SDL_RenderCopy(renderer, rt_ui->left.text, NULL, &rt_ui->left.rect);
	SDL_RenderCopy(renderer, rt_ui->right.text, NULL, &rt_ui->right.rect);
	SDL_RenderCopy(renderer, rt_ui->filters.text, NULL, &rt_ui->filters.rect);
	SDL_RenderCopy(renderer, rt_ui->sound.text, NULL, &rt_ui->sound.rect);
	SDL_RenderCopy(renderer, rt_ui->fault.text, NULL, &rt_ui->fault.rect);
	draw_ui_continue(renderer, rt_ui);
}
