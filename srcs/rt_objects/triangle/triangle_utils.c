/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 02:45:24 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 02:53:19 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			init_norme_triangle(float *r, cl_float3 *v, \
											float *rt, cl_float3 *vt)
{
	rt[0] = r[0];
	rt[1] = r[1];
	rt[2] = r[2];
	vt[0] = v[0];
	vt[1] = v[1];
	vt[2] = v[2];
}

cl_float3		normalize_tr(cl_float3 vec)
{
	float length;

	length = (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return ((cl_float3){{vec.x / length, vec.y / length, vec.z / length}});
}

cl_float3		cross(cl_float3 u, cl_float3 v)
{
	return ((cl_float3){{u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x}});
}

t_basis			get_basis_tr(t_basis basis, cl_float3 *vert)
{
	cl_float3	v1;
	cl_float3	v2;
	cl_float3	normal;

	v1.x = vert[1].x - vert[0].x;
	v1.y = vert[1].y - vert[0].y;
	v1.z = vert[1].z - vert[0].z;
	v2.x = vert[2].x - vert[1].x;
	v2.y = vert[2].y - vert[1].y;
	v2.z = vert[2].z - vert[1].z;
	normal = normalize_tr(cross(v1, v2));
	basis.u = normal;
	basis.w = v1;
	basis.v = cross(basis.u, basis.w);
	return (basis);
}

void			clear_triangle(t_object *obj)
{
	t_triangle *t;

	t = (t_triangle *)obj->data;
	free(t->vertex);
	free(obj->data);
	free(obj);
}
