/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise2d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/21 19:57:01 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/21 20:25:26 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float				dot_top_right(float x, float y, int *new_arr, \
															cl_int2 coord)
{
	cl_float2	top_right;

	top_right.s0 = x - 1.0;
	top_right.s1 = y - 1.0;
	return (dot(top_right, get_constant_vector(new_arr[new_arr[coord.s0 + 1] \
														+ coord.s1 + 1])));
}

float				dot_top_left(float x, float y, int *new_arr, \
															cl_int2 coord)
{
	cl_float2	top_left;

	top_left.s0 = x;
	top_left.s1 = y - 1.0;
	return (dot(top_left, get_constant_vector(new_arr[new_arr[coord.s0] \
														+ coord.s1 + 1])));
}

float				dot_bottom_right(float x, float y, int *new_arr, \
															cl_int2 coord)
{
	cl_float2	bottom_right;

	bottom_right.s0 = x - 1.0;
	bottom_right.s1 = y;
	return (dot(bottom_right, get_constant_vector(new_arr[new_arr[coord.s0 \
														+ 1] + coord.s1])));
}

float				dot_bottom_left(float x, float y, int *new_arr, \
															cl_int2 coord)
{
	cl_float2	bottom_left;

	bottom_left.s0 = x;
	bottom_left.s1 = y;
	return (dot(bottom_left, get_constant_vector(new_arr[new_arr[coord.s0] \
														+ coord.s1])));
}

float				noise2d(float x, float y)
{
	cl_int2		coord;
	cl_float2	u_v;
	int			perlin_arr[512];
	int			*new_arr;

	new_arr = make_permutation(perlin_arr);
	coord.s0 = (int)floor(x) & 255;
	coord.s1 = (int)floor(y) & 255;
	x = x - floor(x);
	y = x - floor(y);
	u_v.s0 = fade(x);
	u_v.s1 = fade(y);
	return (lerp(u_v.s0, lerp(u_v.s1, dot_bottom_left(x, y, new_arr, coord), \
	dot_top_left(x, y, new_arr, coord)), lerp(u_v.s1, \
				dot_bottom_right(x, y, new_arr, coord), \
				dot_top_right(x, y, new_arr, coord))));
}
