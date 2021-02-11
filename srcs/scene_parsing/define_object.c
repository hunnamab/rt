/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_object.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 13:54:29 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/11 22:13:04 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	get_parameters(char *name, char **description, t_scene *scene, int *snmi)
{
	if (ft_strequ(name, "\t\"sphere\":"))
		get_sphere(description, scene, snmi);
	else if (ft_strequ(name, "\t\"triangle\":"))
		get_triangle(description, 0.0, scene, snmi);
	else if (ft_strequ(name, "\t\"plane\":"))
		get_plane(description,  scene, snmi);
	else if (ft_strequ(name, "\t\"cylinder\":"))
		get_cylinder(description,  scene, snmi);
	else if (ft_strequ(name, "\t\"cone\":"))
		get_cone(description,  scene, snmi);
	else if (ft_strequ(name, "\t\"ellipsoid\":"))
		get_ellipsoid(description,  scene, snmi);
	else if (ft_strequ(name, "\t\"box\":"))
		get_box(description,  scene, snmi);
	else if (ft_strequ(name, "\t\"paraboloid\":"))
		get_paraboloid(description,  scene, snmi);
	else if (ft_strequ(name, "\t\"torus\":"))
		get_torus(description,  scene, snmi);
	else if (ft_strequ(name, "\t\"hyperboloid\":"))
		get_hyperboloid(description,  scene, snmi);
	else
		output_error(4);
}

void	one_argument_triangle(char **description, t_scene *scene, int *snmi, float specular)
{
	t_object	*triangle;
	cl_float3	vertex[3];
	t_color		color;
	cl_float3	buf;
	float		rotation[8];

	vertex[0] = get_points(description[1]);
	vertex[1] = get_points(description[2]);
	vertex[2] = get_points(description[3]);
	buf = get_points(description[4]);
	rotation[0] = buf.x;
	rotation[1] = buf.y;
	rotation[2] = buf.z;
	color = get_color(description[5]);
	specular = ftoi(get_coordinates(description[6]));
	rotation[3] = ftoi(get_coordinates(description[7]));
	rotation[4] = ftoi(get_coordinates(description[8]));
	rotation[5] = ftoi(get_coordinates(description[9]));
	rotation[6] = ftoi(get_coordinates(description[10]));
	rotation[7] = ftoi(get_coordinates(description[11]));
	triangle = new_triangle(vertex, specular, color, rotation);
	triangle->text = tex_new_bmp(get_file(description[12]));
	triangle->normal_text = tex_new_bmp(get_file(description[13]));
	scene->objs[snmi[1]] = triangle;
	snmi[1]++;
}

t_object 	*multiple_triangles(char **description, int i, float specular)
{
	t_object	*triangle;
	cl_float3	vertex[3];
	t_color		color;
	cl_float3	buf;
	float		rotation[8];
	
	vertex[0] = get_points(description[i + 1]);
	vertex[1] = get_points(description[i + 2]);
	vertex[2] = get_points(description[i + 3]);
	buf = get_points(description[i + 4]);
	rotation[0] = buf.x;
	rotation[1] = buf.y;
	rotation[2] = buf.z;
	color = get_color(description[i + 5]);
	specular = ftoi(get_coordinates(description[i + 6]));
	rotation[3] = ftoi(get_coordinates(description[i + 7]));
	rotation[4] = ftoi(get_coordinates(description[i + 8]));
	rotation[5] = ftoi(get_coordinates(description[i + 9]));
	rotation[6] = ftoi(get_coordinates(description[i + 10]));
	rotation[7] = ftoi(get_coordinates(description[i + 11]));
	triangle = new_triangle(vertex, specular, color, rotation);
	return (triangle);
}
