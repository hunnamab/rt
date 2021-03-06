/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_second.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 21:41:08 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/19 07:26:05 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3		vector_add(const cl_float3 *v1, const cl_float3 *v2)
{
	cl_float3	res;

	res.x = v1->x + v2->x;
	res.y = v1->y + v2->y;
	res.z = v1->z + v2->z;
	return (res);
}

cl_float3		vector_scale(cl_float3 *v, float c)
{
	cl_float3	res;

	res.x = v->x * c;
	res.y = v->y * c;
	res.z = v->z * c;
	return (res);
}

cl_float3		vector_sub(cl_float3 *v1, cl_float3 *v2)
{
	cl_float3	res;

	res.x = v1->x - v2->x;
	res.y = v1->y - v2->y;
	res.z = v1->z - v2->z;
	return (res);
}

float			vector_dot(cl_float3 *v1, cl_float3 *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

cl_float3		vector_sub_by_scalar(cl_float3 *vector, float scalar)
{
	cl_float3 result;

	result.x = vector->x + scalar;
	result.y = vector->y + scalar;
	result.z = vector->z + scalar;
	return (result);
}
