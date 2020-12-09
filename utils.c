/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 21:37:58 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 13:17:56 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		*protected_malloc(unsigned int size, unsigned int nmb)
{
	void *result;

	if (!(result = malloc(size * nmb)))
	{
		ft_putstr("Malloc error, memory was nor allocated\n");
		exit(0);
	}
	return (result);
}

cl_float3		get_point(float x, float y, float z)
{
	cl_float3 result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

void		copy_point(cl_float3 *dst, cl_float3 *src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

float		choose_t(float t0, float t1)
{
	float result;

	result = 0;
	if ((t1 < t0 && t1 > 0) || (t0 < 0 && t1 >= 0))
		result = t1;
	if ((t0 < t1 && t0 > 0) || (t1 < 0 && t0 >= 0))
		result = t0;
	if (t0 == t1 && t0 >= 0)
		result = t0;
	return (result);
}

void		get_viewport(t_scene *scene)
{
	int		x;
	int		y;
	float	k;

	k = (float)WID / (float)HEI;
	x = 0;
	y = 0;
	while (y < HEI)
	{
		while (x < WID)
		{
			scene->viewport[y * WID + x].y = -(y - (float)HEI / 2) *\
			(1 / (float)HEI) + scene->camera.position.y;
			scene->viewport[y * WID + x].x = (x - (float)WID / 2) *\
			(k / (float)WID) + scene->camera.position.x;
			scene->viewport[y * WID + x].z = scene->camera.position.z + 1;
			x++;
		}
		x = 0;
		y++;
	}
}
