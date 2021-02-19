/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_click_part_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 07:49:52 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 07:59:25 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_rect(SDL_Rect rect, SDL_Event e)
{
	if ((e.button.x >= rect.x && e.button.x <= rect.x + rect.w) && \
					(e.button.y >= rect.y && e.button.y <= rect.y + rect.h))
		return (1);
	return (0);
}

void	am_plus(t_scene *scene)
{
	int i;

	i = 0;
	while (i < scene->light_nmb && scene->light[i].intensity <= 1.0)
	{
		if (scene->light[i].type == AMBIENT)
			scene->light[i].intensity += 0.1;
		i++;
	}
}

void	am_minus(t_scene *scene)
{
	int i;

	i = 0;
	while (i < scene->light_nmb && scene->light[i].intensity >= 0.1)
	{
		if (scene->light[i].type == AMBIENT)
			scene->light[i].intensity -= 0.1;
		i++;
	}
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
