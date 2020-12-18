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

#include "rt_cuda.h"

float3		vector_add(const float3 *v1, const float3 *v2)
{
	float3 res;

	res.x = v1->x + v2->x;
	res.y = v1->y + v2->y;
	res.z = v1->z + v2->z;
	return (res);
}

float3		vector_scale(float3 *v, float c)
{
	float3 res;

	res.x = v->x * c;
	res.y = v->y * c;
	res.z = v->z * c;
	return (res);
}

float3		vector_sub(float3 *v1, float3 *v2)
{
	float3 res;

	res.x = v1->x - v2->x;
	res.y = v1->y - v2->y;
	res.z = v1->z - v2->z;
	return (res);
}

float		vector_dot(float3 *v1, float3 *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

float3		vector_sub_by_scalar(float3 *vector, float scalar)
{
	float3 result;

	result.x = vector->x + scalar;
	result.y = vector->y + scalar;
	result.z = vector->z + scalar;
	return (result);
}
