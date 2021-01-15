/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 19:15:41 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/01/15 15:21:43 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float3 text_map_select(t_object *obj, cl_float3 t)
{
    cl_float3 p;

    if (ft_strequ(obj->tag, "sphere"))
		p = mapping_sphere(t, obj);
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
static cl_float3 		change_basis(cl_float3 vec)
{
	cl_float3
	tmp;

	tmp.x = vec.x * 1.0 + vec.y * 0.0 + vec.z * 0.0;
	tmp.y = vec.x * 0.0 + vec.y * 1.0 + vec.z * 0.0;
	tmp.z = vec.x * 0.0 + vec.y * 0.0 + vec.z * 1.0;
	return (tmp);
}

cl_float3 	mapping_plane(cl_float3 t, t_object *obj)
{
	cl_float3 p;
	t_plane *lol;

	p.x = fabs(fmod(t.x, 1.0));
	p.y = fabs(fmod(t.z, 1.0));
	return (p);
}

cl_float3		mapping_triangle(cl_float3 t, t_object *obj)
{
	cl_float3
 p;
	t_triangle *lol;

	lol = obj->data;
	
	t.x -= lol->vertex[0].x;
	t.y -= lol->vertex[0].y;
	t.z -= lol->vertex[0].z;
	t = change_basis(t);
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
	cl_float3 p;
	t_cone *lol;
	cl_float3 tmp;

	lol = obj->data;
	t.x -= lol->position.x;
	t.y -= lol->position.y;
	t.z -= lol->position.z;
	t = change_basis(t);
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) / M_PI_2;
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
	cl_float3 p;
	t_cylinder *lol;

	lol = obj->data;
	t.x -= lol->position.x;
	t.y -= lol->position.y;
	t.z -= lol->position.z;
	t = change_basis(t);
	float	phi = acos(t.x / lol->radius) / M_PI_2;
	phi = t.z > 0 ? 1.f - phi : phi;
	t.x /= lol->radius;
	t.y /= lol->radius;
	t.z /= lol->radius;	
	p.x = (phi);
	p.y = (-t.y);
	p.z = 0;
	return (p);
	}

cl_float3		normalize(cl_float3 vec)
{
	float length;

	length = (float)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z *vec.z);
	vec.x = vec.x/length;
	vec.y = vec.y/ length;
	vec.z = vec.z/length;
	return (vec);
}

cl_float3		mapping_sphere(cl_float3 t, t_object *obj)
{
    t_sphere *lol;
	cl_float3 p;
	cl_float3 tmp;

	lol = obj->data;
	t.x -= lol->center.x;
	t.y -= lol->center.y;
	t.z -= lol->center.z;
	t = change_basis(t);
	t.x /= lol->radius;
	t.y /= lol->radius;
	t.z /= lol->radius;
	float theta = acos(t.y) / M_PI;
	tmp.x = t.x;
	tmp.y = t.z;
	tmp = normalize(tmp);
	float phi = acos(tmp.x) /M_PI_2;
	phi = t.z > 0 ? 1.f - phi : phi;
	p.x = phi;
	p.y = theta;
	p.z = 0;	
	return (p);
}

