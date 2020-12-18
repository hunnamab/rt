/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:14:20 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/07 15:14:42 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_cuda.h"

void		set_color_zero(t_color *color)
{
	color->red = 0;
	color->green = 0;
	color->blue = 0;
	color->alpha = 0;
}

t_color		set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	t_color color;

	color.red = red;
	color.blue = blue;
	color.green = green;
	color.alpha = alpha;
	return (color);
}

void		copy_color(t_color *dst, t_color *src)
{
	dst->red = src->red;
	dst->blue = src->blue;
	dst->green = src->green;
	dst->alpha = src->alpha;
}

t_color		color_mul_by_scalar(t_color *color, float scalar)
{
	t_color result_color;

	result_color.red = (float)color->red * scalar;
	result_color.blue = (float)color->blue * scalar;
	result_color.green = (float)color->green * scalar;
	return (result_color);
}
