/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:29 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 17:28:32 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	get_rays_arr(t_scene *scene)
{
	int x;
	int y;
	int j;

	x = 0;
	y = 0;
	while (y < HEI)
	{
		while (x < WID)
		{
			j = y * WID + x;
			scene->ray_buf[j].start.x = scene->camera.position.x;
			scene->ray_buf[j].start.y = scene->camera.position.y;
			scene->ray_buf[j].start.z = scene->camera.position.z;
			scene->ray_buf[j].dir = vector_sub(&scene->viewport[j], \
												&scene->camera.position);
			x++;
		}
		x = 0;
		y++;
	}
}

void	get_closest_points(t_scene *scene, double t)
{
	int		ixyj[4];

	ixyj[2] = -1;
	while (++ixyj[2] < HEI)
	{
		ixyj[1] = -1;
		while (++ixyj[1] < WID)
		{
			t = 0;
			ixyj[0] = -1;
			ixyj[3] = ixyj[2] * WID + ixyj[1];
			scene->index_buf[ixyj[3]] = -1;
			scene->depth_buf[ixyj[3]] = 100000000;
			while (++ixyj[0] < scene->obj_nmb)
			{
				t = scene->objs[ixyj[0]]->intersect(&scene->ray_buf[ixyj[3]], \
												scene->objs[ixyj[0]]);
				if (t < scene->depth_buf[ixyj[3]] && t != 0)
				{
					scene->depth_buf[ixyj[3]] = t;
					scene->index_buf[ixyj[3]] = ixyj[0];
				}
			}
		}
	}
}

void	get_intersection_buf(t_scene *scene)
{
	int x;
	int y;
	int i;

	x = 0;
	y = 0;
	while (y < HEI)
	{
		while (x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				scene->intersection_buf[i] = vector_scale(\
				&scene->ray_buf[i].dir, scene->depth_buf[i]);
				scene->intersection_buf[i] = vector_add(\
				&scene->intersection_buf[i], &scene->ray_buf[i].start);
			}
			else
				scene->intersection_buf[i] = get_point(0, 0, 0);
			x++;
		}
		x = 0;
		y++;
	}
}

void	get_normal_buf(t_scene *scene)
{
	int x;
	int y;
	int i;
	int j;

	j = 0;
	x = 0;
	y = 0;
	while (y < HEI)
	{
		while (x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				j = scene->index_buf[i];
				scene->objs[j]->get_normal(scene, i, j);
			}
			else
				scene->normal_buf[j] = get_point(0, 0, 0);
			x++;
		}
		x = 0;
		y++;
	}
}

void	get_material_buf(t_scene *scene)
{
	int x;
	int y;
	int i;

	y = -1;
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				copy_color(&scene->material_buf[i].color, \
							&scene->objs[scene->index_buf[i]]->color);
				scene->material_buf[i].specular = \
				scene->objs[scene->index_buf[i]]->specular;
			}
			else
			{
				set_color_zero(&scene->material_buf[i].color);
				scene->material_buf[i].specular = -1;
			}
		}
	}
}
