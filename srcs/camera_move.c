/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 21:48:47 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/19 09:04:55 by npetrell         ###   ########.fr       */
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
