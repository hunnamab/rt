/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_memory_obj.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 03:13:14 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 03:16:54 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	copy_memory_objects2(t_scene *scene, t_object_d *buf, int i)
{
	if (scene->objs[i]->type == PLANE)
		ft_memcpy(&buf[i].primitive.plane, scene->objs[i]->data, \
													sizeof(t_plane));
	if (scene->objs[i]->type == ELLIPSOID)
		ft_memcpy(&buf[i].primitive.ellipsoid, scene->objs[i]->data, \
												sizeof(t_ellipsoid));
	if (scene->objs[i]->type == PARABOLOID)
		ft_memcpy(&buf[i].primitive.paraboloid, scene->objs[i]->data, \
												sizeof(t_paraboloid));
	if (scene->objs[i]->type == TORUS)
		ft_memcpy(&buf[i].primitive.torus, scene->objs[i]->data, \
												sizeof(t_torus));
	if (scene->objs[i]->type == HYPERBOLOID)
		ft_memcpy(&buf[i].primitive.hyperboloid, \
						scene->objs[i]->data, sizeof(t_hyperboloid));
}

void	copy_memory_objects(t_scene *scene, t_object_d *buf, int i)
{
	if (scene->objs[i]->type == SPHERE)
		ft_memcpy(&buf[i].primitive.sphere, scene->objs[i]->data, \
														sizeof(t_sphere));
	if (scene->objs[i]->type == CONE)
		ft_memcpy(&buf[i].primitive.cone, scene->objs[i]->data, \
													sizeof(t_cone));
	if (scene->objs[i]->type == CYLINDER)
		ft_memcpy(&buf[i].primitive.cylinder, scene->objs[i]->data, \
													sizeof(t_cylinder));
	if (scene->objs[i]->type == TRIANGLE)
		ft_memcpy(&buf[i].primitive.triangle, scene->objs[i]->data, \
													sizeof(t_triangle));
	copy_memory_objects2(scene, buf, i);
}
