/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 22:14:05 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/12 22:21:34 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <OpenCL/opencl.h>

cl_float3		change_basis(cl_float3 vec, t_basis basis)
{
	cl_float3	tmp;

	tmp.x = vec.x * basis.v.x + vec.y * basis.v.y + vec.z * basis.v.z;
	tmp.y = vec.x * basis.u.x + vec.y * basis.u.y + vec.z * basis.u.z;
	tmp.z = vec.x * basis.w.x + vec.y * basis.w.y + vec.z * basis.w.z;
	return (tmp);
}

cl_float3		normalize(cl_float3 vec)
{
	float		length;

	length = (float)sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x = vec.x / length;
	vec.y = vec.y / length;
	vec.z = vec.z / length;
	return (vec);
}

cl_float3		mapping_plane(cl_float3 t, t_object *obj)
{
	cl_float3	p;
	t_plane		*lol;

	p.x = fabs(fmod(t.x, 1.0));
	p.y = fabs(fmod(t.z, 1.0));
	return (p);
}
