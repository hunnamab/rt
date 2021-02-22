/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_parameters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 12:00:26 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 19:01:49 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	get_camera(char **description, t_scene *scene)
{
	t_camera	camera;
	cl_float3	buf;

	camera.position = get_points(description[1]);
	buf = get_points(description[2]);
	camera.rotation[0] = buf.x;
	camera.rotation[1] = buf.y;
	camera.rotation[2] = buf.z;
	scene->camera = camera;
	ft_printf("camera (%f,%f,%f)\n", camera.rotation[0],
			camera.rotation[1], camera.rotation[2]);
}
