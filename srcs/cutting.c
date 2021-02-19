/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 09:37:10 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 09:46:17 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		plane(t_scene *scene, int i, int j)
{
	float		**matrix;

	if (scene->objs[i]->cutting_surfaces[j].type == PLANE)
	{
		scene->objs[i]->cutting_surfaces[j].param2 = \
		vector_sub(&scene->objs[i]->position, \
				&scene->objs[i]->cutting_surfaces[j].param2);
		matrix = get_rotation_matrix(scene->objs[i]->rotation);
		transform(&scene->objs[i]->cutting_surfaces[j].param1,\
													matrix, 1);
		matr_free(matrix, 4);
		scene->objs[i]->cutting_surfaces[j].param3 = \
			-(scene->objs[i]->cutting_surfaces[j].param1.x * \
			scene->objs[i]->cutting_surfaces[j].param2.x) - \
			scene->objs[i]->cutting_surfaces[j].param1.y * \
			scene->objs[i]->cutting_surfaces[j].param2.y - \
			scene->objs[i]->cutting_surfaces[j].param1.z * \
			scene->objs[i]->cutting_surfaces[j].param2.z;
	}
}

static void		sphere(t_scene *scene, int i, int j)
{
	if (scene->objs[i]->cutting_surfaces[j].type == SPHERE)
	{
		scene->objs[i]->cutting_surfaces[j].param1 = \
		vector_sub(&scene->objs[i]->position, \
				&scene->objs[i]->cutting_surfaces[j].param1);
	}
}

static void		cylinder(t_scene *scene, int i, int j)
{
	if (scene->objs[i]->cutting_surfaces[j].type == CYLINDER)
	{
		scene->objs[i]->cutting_surfaces[j].param1 = \
		vector_sub(&scene->objs[i]->position, \
		&scene->objs[i]->cutting_surfaces[j].param1);
		scene->objs[i]->cutting_surfaces[j].param2.x += \
								scene->objs[i]->rotation[0];
		scene->objs[i]->cutting_surfaces[j].param2.y += \
								scene->objs[i]->rotation[1];
		scene->objs[i]->cutting_surfaces[j].param1.z += \
								scene->objs[i]->rotation[2];
	}
}

void			local_cutting(t_scene *scene)
{
	int			i;
	int			j;

	i = -1;
	while (++i < scene->obj_nmb)
	{
		j = -1;
		while (++j < scene->objs[i]->cs_nmb)
		{
			if (scene->objs[i]->cutting_surfaces[j].is_local)
			{
				plane(scene, i, j);
				sphere(scene, i, j);
				cylinder(scene, i, j);
			}
		}
	}
}
