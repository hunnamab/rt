/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 21:57:03 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/21 20:22:03 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

cl_float2			get_constant_vector(float x)
{
	int				h;
	cl_float2		vec;

	h = (int)x & 3;
	if (h == 0)
	{
		vec.x = 1.0;
		vec.y = 1.0;
	}
	else if (h == 1)
	{
		vec.x = -1.0;
		vec.y = 1.0;
	}
	else if (h == 2)
	{
		vec.x = -1.0;
		vec.y = -1.0;
	}
	else
	{
		vec.x = 1.0;
		vec.y = -1.0;
	}
	return (vec);
}

static void			init_perlin(t_texture *texture)
{
	texture->height = 512;
	texture->width = 512;
	texture->size = texture->height * texture->width * 4;
	texture->bytes_per_pixel = 4;
	texture->l_size = 512;
}

t_texture			*calc_perlin(void)
{
	int				x;
	int				y;
	float			noise;
	t_texture		*texture;
	int				color;

	texture = (t_texture *)malloc(sizeof(t_texture));
	init_perlin(texture);
	texture->pixels = (char *)malloc(sizeof(char) * texture->size);
	y = -1;
	while (++y < texture->height)
	{
		x = -1;
		while (++x < texture->width)
		{
			noise = noise2d(x * 0.01, y * 0.01);
			noise = (noise + 1.0) / 2.0;
			color = round(255 * noise);
			texture->pixels[y * texture->width + x] = \
							0xFFFF * noise + 0xFF * noise + noise;
		}
	}
	return (texture);
}
