/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_parameters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:58:35 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 15:38:22 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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

static void	init_norme(t_point *pos_dir, double *intensity)
{
	*intensity = 0.0;
	pos_dir[0].x = 0.0;
	pos_dir[0].y = 0.0;
	pos_dir[0].z = 0.0;
	pos_dir[1].x = 0.0;
	pos_dir[1].y = 0.0;
	pos_dir[1].z = 0.0;
}

t_light		*get_light(char **description)
{
	t_light		*light;
	t_point		pos_dir[2];
	double		intensity;
	char		*type;

	light = NULL;
	type = get_light_type(description[0]); // printf("light type = |%s|\n", type);
	init_norme(pos_dir, &intensity);
	if (ft_strequ(type, "point"))
	{
		pos_dir[0] = get_points(description[1]);
		intensity = ftoi(get_coordinates(description[2]));
	}
	else if (ft_strequ(type, "directional"))
	{
		pos_dir[1] = get_points(description[1]);
		intensity = ftoi(get_coordinates(description[2]));
	}
	else if (ft_strequ(type, "ambient"))
		intensity = ftoi(get_coordinates(description[1]));
	else
		output_error(5);
	light = new_light(pos_dir, type, intensity);
	free(type);
	return (light);
}
