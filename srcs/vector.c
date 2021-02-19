/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 21:39:04 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/19 07:25:22 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3		vector_div_by_scalar(cl_float3 *vector, float scalar)
{
	cl_float3	result;

	result.x = vector->x / scalar;
	result.y = vector->y / scalar;
	result.z = vector->z / scalar;
	return (result);
}

float			vector_length(cl_float3 *vector)
{
	float		dot;

	dot = vector_dot(vector, vector);
	return (sqrt(dot));
}

void			normalize_vector(cl_float3 *v1)
{
	vector_div_by_scalar(&v1[0], \
	vector_length(&v1[0]));
}

cl_float3		vector_cross(cl_float3 *v1, cl_float3 *v2)
{
	cl_float3	result;

	result.x = v1->y * v2->z - v1->z * v2->y;
	result.y = v1->z * v2->x - v1->x * v2->z;
	result.z = v1->x * v2->y - v1->y * v2->x;
	return (result);
}

cl_float3		vector_add_scalar(const cl_float3 *v1, const float scalar)
{
	cl_float3	res;

	res.x = v1->x + scalar;
	res.y = v1->y + scalar;
	res.z = v1->z + scalar;
	return (res);
}
