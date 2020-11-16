/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 13:54:29 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 15:23:33 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_object	*get_parameters(char *name, char **description)
{
	t_object	*obj;

	obj = NULL;
	if (ft_strequ(name, "sphere")) //printf("sphere\n");
		obj = get_sphere(description); // objects_parameters.c
	else if (ft_strequ(name, "triangle")) //printf("triangle\n");
		obj = get_triangle(description, 0.0);
	else if (ft_strequ(name, "plane")) //printf("plane\n");
		obj = get_plane(description);
	else if (ft_strequ(name, "cylinder")) //printf("cylinder\n");
		obj = get_cylinder(description);
	else if (ft_strequ(name, "cone")) //printf("cone\n");
		obj = get_cone(description);
	else
		output_error(4);
	return (obj);
}
