/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_parameters.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:22 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/12 18:05:51 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	get_sphere(char **description, t_scene *scene, int *snmi)
{
	t_object	*sphere;
	int i;
	
	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				sphere = multiple_spheres(description, scene, snmi, i);
				sphere->text = tex_new_bmp(get_file(description[i + 6]));
				scene->objs[snmi[1]] = sphere;
				snmi[1]++;
				i += 8;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_sphere(description, scene, snmi);
}

void	get_cylinder(char **description, t_scene *scene, int *snmi)
{
	t_object	*cylinder;
	int i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				cylinder = multiple_cylinders(description, scene, snmi, i);
				cylinder->text = tex_new_bmp(get_file(description[i + 7]));
				scene->objs[snmi[1]] = cylinder;
				snmi[1]++;
				i += 9;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_cylinder(description, scene, snmi);
}

void	get_cone(char **description, t_scene *scene, int *snmi)
{
	t_object	*cone;
	int i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				cone = multiple_cones(description, scene, snmi, i);
				cone->text = tex_new_bmp(get_file(description[i + 7]));
				scene->objs[snmi[1]] = cone;
				snmi[1]++;
				i += 9;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_cone(description, scene, snmi);
}


void	get_triangle(char **description, double specular, t_scene *scene, int *snmi)
{
	t_object	*triangle;
	int i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				triangle = multiple_triangles(description, snmi, i, specular);
				triangle->text = tex_new_bmp(get_file(description[i + 7]));
				scene->objs[snmi[1]] = triangle;
				snmi[1]++;
				i += 9;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_triangle(description, scene, snmi, specular);
}

void	get_plane(char **description,  t_scene *scene, int *snmi)
{
	t_object	*plane;
	int i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				plane = multiple_planes(description, scene, snmi, i);
				plane->text = tex_new_bmp(get_file(description[i + 6]));
				scene->objs[snmi[1]] = plane;
				snmi[1]++;
				i += 6;
			}
		}
	}
	if (description[0][0] == '{')
		one_argument_plane(description, scene, snmi);
}
