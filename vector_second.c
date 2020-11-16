/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_second.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 21:41:08 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/09 13:07:38 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_point		vector_add(const t_point *v1, const t_point *v2)
{
	t_point res;

	res.x = v1->x + v2->x;
	res.y = v1->y + v2->y;
	res.z = v1->z + v2->z;
	return (res);
}

t_point		vector_scale(t_point *v, double c)
{
	t_point res;

	res.x = v->x * c;
	res.y = v->y * c;
	res.z = v->z * c;
	return (res);
}

t_point		vector_sub(t_point *v1, t_point *v2)
{
	t_point res;

	res.x = v1->x - v2->x;
	res.y = v1->y - v2->y;
	res.z = v1->z - v2->z;
	return (res);
}

double		vector_dot(t_point *v1, t_point *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

t_point		vector_sub_by_scalar(t_point *vector, double scalar)
{
	t_point result;

	result.x = vector->x + scalar;
	result.y = vector->y + scalar;
	result.z = vector->z + scalar;
	return (result);
}
