/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_parameters_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/20 22:22:20 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/21 15:02:11 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int					add_count(char *type, int i)
{
	if (ft_strequ(type, "\"point\","))
		i += 5;
	else if (ft_strequ(type, "\"directional\","))
		i += 6;
	else
		i += 4;
	return (i);
}

void				new_directional_light(char **description,
		t_scene *scene, int *snmi)
{
	t_light			light;
	cl_float3		pos_dir[2];
	double			intensity;
	int				new_type;

	pos_dir[0] = get_points(description[2]);
	intensity = ftoi(get_coordinates(description[3]));
	pos_dir[1] = get_points(description[4]);
	new_type = DIRECTIONAL;
	light = new_light(pos_dir, new_type, intensity);
	scene->light[snmi[2]] = light;
	snmi[2]++;
}
