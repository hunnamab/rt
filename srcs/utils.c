/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 21:37:58 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/19 07:27:25 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			*protected_malloc(unsigned int size, unsigned int nmb)
{
	void		*result;

	if (!(result = malloc(size * nmb)))
	{
		ft_putstr("Malloc error, memory was nor allocated\n");
		exit(0);
	}
	return (result);
}

cl_float3		get_point(float x, float y, float z)
{
	cl_float3	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

void			copy_point(cl_float3 *dst, cl_float3 *src)
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

float			choose_t(float t0, float t1)
{
	float		result;

	result = 0;
	if ((t1 < t0 && t1 > 0) || (t0 < 0 && t1 >= 0))
		result = t1;
	if ((t0 < t1 && t0 > 0) || (t1 < 0 && t0 >= 0))
		result = t0;
	if (t0 == t1 && t0 >= 0)
		result = t0;
	return (result);
}
