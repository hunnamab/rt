/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_rotate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 02:13:17 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 02:23:33 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3	clvec_rot_x(cl_float3 v, double a)
{
	cl_float3	r;

	r.x = v.x;
	r.y = (float)(v.y * cos(a) - v.z * sin(a));
	r.z = (float)(v.y * sin(a) + v.z * cos(a));
	return (r);
}

cl_float3	clvec_rot_y(cl_float3 v, double a)
{
	cl_float3	r;

	r.x = (float)(v.x * cos(a) + v.z * sin(a));
	r.y = v.y;
	r.z = (float)(v.z * cos(a) - v.x * sin(a));
	return (r);
}

cl_float3	clvec_rot_z(cl_float3 v, double a)
{
	cl_float3	r;

	r.x = (float)(v.x * cos(a) - v.y * sin(a));
	r.y = (float)(v.x * sin(a) + v.y * cos(a));
	r.z = v.z;
	return (r);
}

t_basis		get_basis(t_basis basis, float *rot)
{
	basis.u = clvec_rot_x(basis.u, (rot[0]));
	basis.u = clvec_rot_y(basis.u, (rot[1]));
	basis.u = clvec_rot_z(basis.u, (rot[2]));
	basis.v = clvec_rot_x(basis.v, (rot[0]));
	basis.v = clvec_rot_y(basis.v, (rot[1]));
	basis.v = clvec_rot_z(basis.v, (rot[2]));
	basis.w = clvec_rot_x(basis.w, (rot[0]));
	basis.w = clvec_rot_y(basis.w, (rot[1]));
	basis.w = clvec_rot_z(basis.w, (rot[2]));
	return (basis);
}

t_basis		get_default(t_basis basis)
{
	basis.u = (cl_float3){{0.0, 1.0, 0.0}};
	basis.v = (cl_float3){{1.0, 0.0, 0.0}};
	basis.w = (cl_float3){{0.0, 0.0, 1.0}};
	return (basis);
}
