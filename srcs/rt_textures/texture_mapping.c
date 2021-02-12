/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:15:41 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/12 22:19:52 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include <OpenCL/opencl.h>

cl_float3		text_map_select(t_object *obj, cl_float3 t, int index)
{
	cl_float3	p;

	if (ft_strequ(obj->tag, "sphere"))
		p = mapping_sphere(t, obj, index);
	if (ft_strequ(obj->tag, "plane"))
		p = mapping_plane(t, obj);
	if (ft_strequ(obj->tag, "cylinder"))
		p = mapping_cylinder(t, obj);
	if (ft_strequ(obj->tag, "cone"))
		p = mapping_cone(t, obj);
	if (ft_strequ(obj->tag, "triangle"))
		p = mapping_triangle(t, obj);
	return (p);
}

cl_float3		mapping_triangle(cl_float3 t, t_object *obj)
{
	cl_float3	p;
	t_triangle	*lol;

	lol = obj->data;
	t.x -= lol->vertex[0].x;
	t.y -= lol->vertex[0].y;
	t.z -= lol->vertex[0].z;
	t = change_basis(t, obj->basis);
	t.x /= 10;
	t.y /= 10;
	t.z /= 10;
	p.x = (t.x);
	p.y = (t.z);
	p.z = 0;
	return (p);
}

cl_float3		mapping_cone(cl_float3 t, t_object *obj)
{
	cl_float3	p;
	t_cone		*lol;
	cl_float3	tmp;
	float		phi;

	lol = obj->data;
	t.x -= lol->position.x;
	t.y -= lol->position.y;
	t.z -= lol->position.z;
	t = change_basis(t, obj->basis);
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	phi = acos(tmp.x) / M_PI_2;
	phi = tmp.y > 0 ? 1.f - phi : phi;
	t.x /= 20;
	t.y /= 20;
	t.z /= 20;
	p.x = fabs(-phi);
	p.y = fabs(t.y);
	p.z = 0;
	return (p);
}

cl_float3		mapping_cylinder(cl_float3 t, t_object *obj)
{
	cl_float3	p;
	t_cylinder	*lol;
	float		phi;

	lol = obj->data;
	t.x -= lol->position.x;
	t.y -= lol->position.y;
	t.z -= lol->position.z;
	t = change_basis(t, obj->basis);
	phi = acos(t.x / lol->radius) / M_PI_2;
	phi = t.z > 0 ? 1.f - phi : phi;
	t.x /= lol->radius;
	t.y /= lol->radius;
	t.z /= lol->radius;
	p.x = (phi);
	p.y = (-t.y);
	p.z = 0;
	return (p);
}

cl_float3		mapping_sphere(cl_float3 t, t_object *obj, int index)
{
	t_sphere	*lol;
	cl_float3	p;
	cl_float3	tmp;
	float		theta;
	float		phi;

	lol = obj->data;
	t.x -= lol->center.x;
	t.y -= lol->center.y;
	t.z -= lol->center.z;
	t = change_basis(t, obj->basis);
	t.x /= lol->radius;
	t.y /= lol->radius;
	t.z /= lol->radius;
	theta = acos(t.y) / M_PI;
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	phi = acos(tmp.x) / M_PI_2;
	phi = t.z > 0 ? 1.f - phi : phi;
	p.x = phi;
	p.y = theta;
	p.z = 0;
	return (p);
}
