/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 19:36:25 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/21 20:18:23 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float	dot(cl_float2 vector1, cl_float2 vector2)
{
	return (vector1.s0 * vector2.s0 + vector1.s1 * vector2.s1);
}

int		*shuffle(int perlin_arr[512])
{
	int i;
	int	index;
	int	temp;

	i = 255;
	while (i > 0)
	{
		index = rand() % 255;
		temp = perlin_arr[i];
		perlin_arr[i] = perlin_arr[index];
		perlin_arr[index] = temp;
		i--;
	}
	return (perlin_arr);
}

int		*make_permutation(int perlin_arr[512])
{
	int	i;

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
	return (perlin_arr);
}

float	fade(float t)
{
	return (((6 * t - 15) * t + 10) * t * t * t);
}

float	lerp(float t, float a1, float a2)
{
	return (a1 + t * (a2 - a1));
}
