/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_parameters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:58:35 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 05:08:22 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			init_norme(cl_float3 *pos_dir, float *intensity,
																int *new_type)
{
	*intensity = 0.0;
	pos_dir[0].x = 0.0;
	pos_dir[0].y = 0.0;
	pos_dir[0].z = 0.0;
	pos_dir[1].x = 0.0;
	pos_dir[1].y = 0.0;
	pos_dir[1].z = 0.0;
	*new_type = AMBIENT;
}

static void			one_light(char **description, t_scene *scene, \
												int *snmi, int new_type)
{
	t_light			light;
	cl_float3		pos_dir[2];
	float			intensity;
	char			*type;

	type = get_light_type(description[1]);
	init_norme(pos_dir, &intensity, &new_type);
	if (ft_strequ(type, "\"point\","))
	{
		pos_dir[0] = get_points(description[2]);
		intensity = ftoi(get_coordinates(description[3]));
		new_type = POINT;
	}
	else if (ft_strequ(type, "\"directional\","))
	{
		new_directional_light(description, scene, snmi);
		return ;
	}
	else if (ft_strequ(type, "\"ambient\","))
		intensity = ftoi(get_coordinates(description[2]));
	else
		output_error(5);
	light = new_light(pos_dir, new_type, intensity);
	scene->light[snmi[2]++] = light;
	free(type);
}

t_light				many_lights(char **description, int *snmi, \
												int i, char *type)
{
	cl_float3		pos_dir[2];
	float			intensity;
	int				new_type;
	int				tmp;

	tmp = *snmi;
	init_norme(pos_dir, &intensity, &new_type);
	if (ft_strequ(type, "\"point\","))
	{
		pos_dir[0] = get_points(description[i + 2]);
		intensity = ftoi(get_coordinates(description[i + 3]));
		new_type = POINT;
	}
	else if (ft_strequ(type, "\"directional\","))
	{
		pos_dir[0] = get_points(description[i + 2]);
		intensity = ftoi(get_coordinates(description[i + 3]));
		pos_dir[1] = get_points(description[i + 4]);
		new_type = DIRECTIONAL;
	}
	else if (ft_strequ(type, "\"ambient\","))
		intensity = ftoi(get_coordinates(description[i + 2]));
	else
		output_error(5);
	return (new_light(pos_dir, new_type, intensity));
}

void				get_light(char **description, t_scene *scene, int *snmi)
{
	t_light			light;
	char			*type;
	int				i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				type = get_light_type(description[i + 1]);
				light = many_lights(description, snmi, i, type);
				scene->light[snmi[2]] = light;
				snmi[2]++;
				i = add_count(type, i);
				free(type);
			}
		}
	}
	if (description[0][0] == '{')
		one_light(description, scene, snmi, 0);
}
