/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_parameters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:22 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 15:20:59 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_object	*get_sphere(char **description)
{
	t_object	*sphere;
	t_point		cen_buf[2];
	t_color		color;
	double		rotation[3];
	double		rad_spec[2];

	cen_buf[0] = get_points(description[0]);
	rad_spec[0] = ftoi(get_coordinates(description[1]));
	cen_buf[1] = get_points(description[2]);
	rotation[0] = cen_buf[1].x;
	rotation[1] = cen_buf[1].y;
	rotation[2] = cen_buf[1].z;
	color = get_color(description[3]);
	rad_spec[1] = ftoi(get_coordinates(description[4]));
	sphere = new_sphere(cen_buf[0], rad_spec, color, rotation);
	return (sphere);
}

t_object	*get_cylinder(char **description)
{
	t_object	*cylinder;
	t_color		color;
	t_point		pos_vec_buf[3];
	double		rotation[3];
	double		rad_spec[2];

	pos_vec_buf[0] = get_points(description[0]);
	rad_spec[0] = ftoi(get_coordinates(description[1]));
	pos_vec_buf[1] = get_points(description[2]);
	pos_vec_buf[2] = get_points(description[3]);
	rotation[0] = pos_vec_buf[2].x;
	rotation[1] = pos_vec_buf[2].y;
	rotation[2] = pos_vec_buf[2].z;
	color = get_color(description[4]);
	rad_spec[1] = ftoi(get_coordinates(description[5]));
	cylinder = new_cylinder(pos_vec_buf, rad_spec, color, rotation);
	return (cylinder);
}

t_object	*get_cone(char **description)
{
	t_object	*cone;
	t_color		color;
	t_point		pos_vec_buf[3];
	double		rotation[3];
	double		ang_spec[2];

	pos_vec_buf[0] = get_points(description[0]);
	pos_vec_buf[1] = get_points(description[1]);
	ang_spec[0] = ftoi(get_coordinates(description[2]));
	pos_vec_buf[2] = get_points(description[3]);
	rotation[0] = pos_vec_buf[2].x;
	rotation[1] = pos_vec_buf[2].y;
	rotation[2] = pos_vec_buf[2].z;
	color = get_color(description[4]);
	ang_spec[1] = ftoi(get_coordinates(description[5]));
	cone = new_cone(pos_vec_buf, ang_spec, color, rotation);
	return (cone);
}

t_object	*get_triangle(char **description, double specular)
{
	t_object	*triangle;
	t_point		vertex[3];
	t_color		color;
	t_point		buf;
	double		rotation[3];

	vertex[0] = get_points(description[0]);
	vertex[1] = get_points(description[1]);
	vertex[2] = get_points(description[2]);
	buf = get_points(description[3]);
	rotation[0] = buf.x;
	rotation[1] = buf.y;
	rotation[2] = buf.z;
	color = get_color(description[4]);
	specular = ftoi(get_coordinates(description[5]));
	triangle = new_triangle(vertex, specular, color, rotation);
	return (triangle);
}

t_object	*get_plane(char **description)
{
	t_object	*plane;
	t_color		color;
	t_point		poi_nor_buf[3];
	double		specular;
	double		rotation[3];

	poi_nor_buf[0] = get_points(description[0]);
	poi_nor_buf[1] = get_points(description[1]);
	poi_nor_buf[2] = get_points(description[2]);
	rotation[0] = poi_nor_buf[2].x;
	rotation[1] = poi_nor_buf[2].y;
	rotation[2] = poi_nor_buf[2].z;
	color = get_color(description[3]);
	specular = ftoi(get_coordinates(description[4]));
	plane = new_plane(poi_nor_buf, specular, color, rotation);
	return (plane);
}
