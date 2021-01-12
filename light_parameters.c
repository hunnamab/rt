/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_parameters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:58:35 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/12 18:30:44 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char		*get_light_type(char *description)
{
	char	*type;
	char	*buf;
	int		i;
	int		len;

	if (!description)
		output_error(5);
	i = 0;
	len = ft_strlen(description);
	while (description[i] != ':' && description[i] != '\0')
		i++;
	i++;
	type = ft_strsub(description, i, len - i);
	buf = ft_strtrim(type);
	free(type);
	return (buf);
}

static void	init_norme(cl_float3 *pos_dir, float *intensity)
{
	*intensity = 0.0;
	pos_dir[0].x = 0.0;
	pos_dir[0].y = 0.0;
	pos_dir[0].z = 0.0;
	pos_dir[1].x = 0.0;
	pos_dir[1].y = 0.0;
	pos_dir[1].z = 0.0;
}

static void	one_light(char **description, t_scene *scene, int * snmi)
{
	t_light		*light;
	cl_float3	pos_dir[2];
	double		intensity;
	char		*type;
	
	type = get_light_type(description[1]); // printf("light type = |%s|\n", type);
	init_norme(pos_dir, &intensity);
	if (ft_strequ(type, "\"point\","))
	{
		pos_dir[0] = get_points(description[2]);
		intensity = ftoi(get_coordinates(description[3]));
	}
	else if (ft_strequ(type, "\"directional\","))
	{
		pos_dir[1] = get_points(description[2]);
		intensity = ftoi(get_coordinates(description[3]));
	}
	else if (ft_strequ(type, "\"ambient\","))
		intensity = ftoi(get_coordinates(description[2]));
	else
		output_error(5);
	light = new_light(pos_dir, type, intensity);
	scene->light[snmi[2]] = light;
	snmi[2]++;
	free(type);
}

static t_light		*many_lights(char **description, int *snmi, int i, char *type)
{
	t_light		*light;
	cl_float3	pos_dir[2];
	double		intensity;

	init_norme(pos_dir, &intensity);
	if (ft_strequ(type, "\"point\","))
	{
		pos_dir[0] = get_points(description[i + 2]);
		intensity = ftoi(get_coordinates(description[i + 3]));
	}
	else if (ft_strequ(type, "\"directional\","))
	{
		pos_dir[1] = get_points(description[i + 2]);
		intensity = ftoi(get_coordinates(description[i + 3]));
	}
	else if (ft_strequ(type, "\"ambient\","))
		intensity = ftoi(get_coordinates(description[i + 2]));
	else
		output_error(5);
	light = new_light(pos_dir, type, intensity);
	return (light);
}

void	get_light(char **description, t_scene *scene, int * snmi)
{
	t_light		*light;
	cl_float3	pos_dir[2];
	double		intensity;
	char		*type;
	int 		i;

	light = NULL;
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
				if (ft_strequ(type, "\"point\",") || ft_strequ(type, "\"directional\","))
					i += 5;
				else
					i += 4;
				free(type);
			}
		}
	}
	if (description[0][0] == '{')
		one_light(description, scene, snmi);
}
