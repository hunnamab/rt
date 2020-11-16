/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameters_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 12:02:06 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/09 12:56:47 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

char		*get_coordinates(char *description)
{
	char	*coordinates;
	int		i;
	int		len;

	if (!description)
		output_error(5);
	i = 0;
	len = ft_strlen(description);
	while (!(ft_isdigit(description[i])) && description[i] != '-')
	{
		if (description[i + 1] == '\0')
			output_error(5);
		i++;
	}
	if (!(coordinates = ft_strsub(description, i, len - i)))
		output_error(5);
	return (coordinates);
}

t_color		get_color(char *description)
{
	char	*coord_buf;
	char	**coordinates;
	t_color	color;

	if (!description)
		output_error(5);
	coord_buf = get_coordinates(description);
	if (!(coordinates = ft_strsplit(coord_buf, ',')))
		output_error(5);
	free(coord_buf);
	if (!coordinates[0] || !coordinates[1] || \
		!coordinates[2] || !coordinates[3])
		output_error(5);
	color.red = ft_atoi(coordinates[0]);
	color.green = ft_atoi(coordinates[1]);
	color.blue = ft_atoi(coordinates[2]);
	color.alpha = ft_atoi(coordinates[3]);
	ft_memdel_double((void **)coordinates);
	return (color);
}

t_point		get_points(char *description)
{
	char	*coord_buf;
	char	**coordinates;
	t_point	points;

	if (!description)
		output_error(5);
	printf("description in get_points = |%s|\n", description);
	coord_buf = get_coordinates(description);
	if (!(coordinates = ft_strsplit(coord_buf, ',')))
		output_error(5);
	free(coord_buf);
	if (!coordinates[0] || !coordinates[1] || !coordinates[2])
		output_error(5);
	points.x = ftoi(ft_strtrim(coordinates[0]));
	printf("x = %f\n", points.x);
	points.y = ftoi(ft_strtrim(coordinates[1]));
	printf("y = %f\n", points.y);
	points.z = ftoi(ft_strtrim(coordinates[2]));
	printf("z = %f\n", points.z);
	ft_memdel_double((void **)coordinates);
	return (points);
}
