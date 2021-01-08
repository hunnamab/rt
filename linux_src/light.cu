/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:23:24 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/11 12:32:22 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern "C"{
	#include "rt_host.h"
	}
	#include "rt_device.cuh"

__device__ t_color	color_mul_by_scalar(t_color *color, float scalar)
{
	t_color result_color;

	result_color.red = (float)color->red * scalar;
	result_color.blue = (float)color->blue * scalar;
	result_color.green = (float)color->green * scalar;
	return (result_color);
}


t_light		*new_light(float3 *pos_dir, int type, float intensity)
{
	t_light *new_light;

	new_light = (t_light *)protected_malloc(sizeof(t_light), 1);
	new_light->type = type;
	new_light->intensity = intensity;
	new_light->position = pos_dir[0];
	new_light->direction = pos_dir[1];
	return (new_light);
}

__device__ float3	get_light_vec(t_light *light, float3 *intersection_buf, int index, int j)
{
	float3 light_vec;

	light_vec.x = 0;
	light_vec.y = 0;
	light_vec.z = 0;
	if (light[j].type == POINT)
		light_vec = sub(light[j].position, \
		intersection_buf[index]);
	if (light[j].type == DIRECTIONAL)
		light_vec = light[j].direction;
	return (light_vec);
}

__device__ int			in_shadow(int index, float3 l)
{
/* 	float3	ray_dir;
	float3	ray_start;
	int		i;
	float	t;

	i = 0;
	ray_dir = l;
	ray_start = scale(&ray_dir, 0.0001);
	ray_start = add(&ray_start, &scene->intersection_buf[index]);
	while (i < scene->obj_nmb)
	{
		t = scene->objs[i]->intersect(scene, i, &ray_start, &ray_dir);
		if (t < 1 && t > 0.0001)
			break ;
		i++;
	}
	if (t < 1 && t > 0.0001)
		return (1); */
	return (0);
}

__device__ float		get_specular(float3 *normal_buf, float3 *ray_buf, t_material *material_buf, t_light *light, int index, int j, float3 l)
{
 	float		nri[3];
	float3		r;
	float3		d;
	float3		lb;

	lb = div_by_scalar(l, length(l));
	nri[2] = 0;
	nri[0] = dot(normal_buf[index], lb);
	r = scale(normal_buf[index], 2.0);
	r = scale(r, nri[0]);
	r = sub(r, lb);
	d.x = -ray_buf[index].x;
	d.y = -ray_buf[index].y;
	d.z = -ray_buf[index].z;
	nri[1] = dot(r, d);
	if (nri[1] > 0)
		nri[2] += light[j].intensity * pow((float)nri[1] / \
		(length(r) * length(d)), \
		material_buf[index].specular);
	return (nri[2]); 
}

__device__ t_color		reflection_color(float3 *ray_buf, float3 *normal_buf, t_material *material_buf, t_light *light, float3 *intersection_buf, t_object_d *objs, int index, int light_nmb)
{
 	float	i;
	float3	l;
	float	n_dot_l;
	int		j;

	j = -1;
	i = 0;
	while (++j < light_nmb)
	{
		if (light[j].type == AMBIENT)
			i += light[j].intensity;
		else
		{
			l = get_light_vec(light, intersection_buf, index, j);
			n_dot_l = dot(normal_buf[index], l);
			if (!(in_shadow(index, l)) && n_dot_l > 0)
			{
				if (material_buf[index].specular != -1)
					i += get_specular(normal_buf, ray_buf, material_buf, light, index, j, l);
				i += (light[j].intensity * n_dot_l) / length(l);
			}
		}
	}
	i = i > 1 ? 1 : i; 
	return (color_mul_by_scalar(&material_buf[index].color, i));
}
