/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 21:57:03 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/20 22:51:41 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

typedef struct		s_vector2
{
    float					x;
	float					y;
}					t_vector2;

float				dot(t_vector2 vector1, t_vector2 vector2)
{
	return (vector1.x * vector2.x +  vector1.y * vector2.y);
}

int 			*shuffle(int perlin_arr[512])
{
	int 		i;
	int			index;
	int			temp;

	i = 255;
	while (i > 0)
	{
		index = rand() % 255;
		temp = perlin_arr[i];
		perlin_arr[i] = perlin_arr[index];
		perlin_arr[index] = temp;
		i--;
	}
	return perlin_arr;
}

int 			*make_permutation(int perlin_arr[512])
{
	int			i;

	i = 0;
	while (i < 256)
	{
		perlin_arr[i] = i;
		i++;
	}
	perlin_arr = shuffle(perlin_arr);
	while (i < 512)
	{
		perlin_arr[i] = perlin_arr[i];
		i++;
	}
	return perlin_arr;
}

t_vector2			get_constant_vector(float x)
{
	int 			h;
	t_vector2		vec;

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
	return vec;
}

float 				fade(float t){
	return (((6 * t - 15) * t + 10) * t * t * t);
}

float					lerp(float t, float a1, float a2){
	return (a1 + t * (a2 - a1));
}


float			noise2d(float x, float y)
{
	int		x_coord;
	int		y_coord;
	float		x_f;
	float		y_f;
	t_vector2	top_right;
	t_vector2	top_left;
	t_vector2	bottom_right;
	t_vector2	bottom_left;
	float value_top_right;
	float value_top_left;
	float value_bottom_right;
	float value_bottom_left;
	float dot_top_right;
	float dot_top_left;
	float dot_bottom_right;
	float dot_bottom_left;
	float	u;
	float	v;

	int			perlin_arr[512];
	int			*new_arr;

	new_arr = make_permutation(perlin_arr);
	x_coord = (int)floor(x) & 255;
	y_coord = (int)floor(y) & 255;
	x_f = x - floor(x);
	y_f = x - floor(y);
	top_right.x = x_f - 1.0;
	top_right.y = y_f - 1.0;
	top_left.x = x_f;
	top_left.y = y_f - 1.0;
	bottom_right.x = x_f - 1.0;
	bottom_right.y = y_f;
	bottom_left.x = x_f;
	bottom_left.y = y_f;

	value_top_right = new_arr[new_arr[x_coord+1]+y_coord+1];
	value_top_left = new_arr[new_arr[x_coord]+y_coord+1];
	value_bottom_right = new_arr[new_arr[x_coord+1]+y_coord];
	value_bottom_left = new_arr[new_arr[x_coord]+y_coord];
	dot_top_right = dot(top_right, get_constant_vector(value_top_right));
	dot_top_left = dot(top_left, get_constant_vector(value_top_left));
	dot_bottom_right = dot(bottom_right, get_constant_vector(value_bottom_right));
	dot_bottom_left = dot(bottom_left, get_constant_vector(value_bottom_left));
	
	u = fade(x_f);
	v = fade(y_f);
	
	return lerp(u, lerp(v, dot_bottom_left, dot_top_left),
		lerp(v, dot_bottom_right, dot_top_right)
	);
}


t_texture			*calc_perlin()
{
	int				x;
	int				y;
	float			noise;
    t_texture		*texture;
	int				color;

	texture = (t_texture *)malloc(sizeof(t_texture));
	texture->height = 512;
	texture->width = 512;
	texture->size = texture->height * texture->width * 4;
	texture->bytes_per_pixel = 4;
	texture->l_size = 512;
	texture->pixels = (char *)malloc(sizeof(char) * texture->size);
	y = 0;
	while (y < texture->height)
	{
		x = 0;
		while (x < texture->width)
		{
			noise = noise2d(x * 0.01, y * 0.01);
			noise += 1.0;
			noise /= 2.0;
			color = round(255 * noise);
			texture->pixels[y * texture->width + x] = 0xFFFF * noise + 0xFF * noise + noise;
			x++;
		}
		y++;
	}
	return (texture);
}