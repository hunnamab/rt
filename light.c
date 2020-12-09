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

#include "rt.h"

t_light		*new_light(cl_float3 *pos_dir, const char *type, float intensity)
{
	t_light *new_light;

	new_light = protected_malloc(sizeof(t_light), 1);
	new_light->type = (const char *)ft_strcpy_alloc(\
	(char *)new_light->type, (char *)type);
	new_light->intensity = intensity;
	new_light->position = pos_dir[0];
	new_light->direction = pos_dir[1];
	return (new_light);
}

cl_float3		get_light_vec(t_scene *scene, int index, int j)
{
	cl_float3 light_vec;

	light_vec = get_point(0, 0, 0);
	if (ft_strequ(scene->light[j]->type, "point"))
		light_vec = vector_sub(&scene->light[j]->position, \
		&scene->intersection_buf[index]);
	if (ft_strequ(scene->light[j]->type, "directional"))
		light_vec = scene->light[j]->direction;
	return (light_vec);
}

int			in_shadow(t_scene *scene, int index, cl_float3 l)
{
	t_ray	ray;
	int		i;
	float	t;

	i = 0;
	ray.dir = l;
	ray.start = vector_scale(&ray.dir, 0.0001);
	ray.start = vector_add(&ray.start, &scene->intersection_buf[index]);
	while (i < scene->obj_nmb)
	{
		t = scene->objs[i]->intersect(&ray, scene->objs[i]);
		if (t < 1 && t > 0.0001)
			break ;
		i++;
	}
	if (t < 1 && t > 0.0001)
		return (1);
	return (0);
}

float		get_specular(t_scene *scene, int index, int j, cl_float3 *l)
{
	float		nri[3];
	cl_float3		r;
	cl_float3		d;
	cl_float3		lb;

	lb = vector_div_by_scalar(l, vector_length(l));
	nri[2] = 0;
	nri[0] = vector_dot(&scene->normal_buf[index], &lb);
	r = vector_scale(&scene->normal_buf[index], 2.0);
	r = vector_scale(&r, nri[0]);
	r = vector_sub(&r, &lb);
	d.x = -scene->ray_buf[index].dir.x;
	d.y = -scene->ray_buf[index].dir.y;
	d.z = -scene->ray_buf[index].dir.z;
	nri[1] = vector_dot(&r, &d);
	if (nri[1] > 0)
		nri[2] += scene->light[j]->intensity * pow((float)nri[1] / \
		(vector_length(&r) * vector_length(&d)), \
		scene->material_buf[index].specular);
	return (nri[2]);
}

t_color		reflection_color(t_scene *scene, int index)
{
	float	i;
	cl_float3	l;
	float	n_dot_l;
	int		j;

	j = -1;
	i = 0;
	while (++j < scene->light_nmb)
	{
		if (ft_strequ(scene->light[j]->type, "ambient"))
			i += scene->light[j]->intensity;
		else
		{
			l = get_light_vec(scene, index, j);
			n_dot_l = vector_dot(&scene->normal_buf[index], &l);
			if (!(in_shadow(scene, index, l)) && n_dot_l > 0)
			{
				if (scene->material_buf[index].specular != -1)
					i += get_specular(scene, index, j, &l);
				i += (scene->light[j]->intensity * n_dot_l) / vector_length(&l);
			}
		}
	}
	i = i > 1 ? 1 : i;
	return (color_mul_by_scalar(&scene->material_buf[index].color, i));
}
