/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_move2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 21:49:40 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/19 09:04:51 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
