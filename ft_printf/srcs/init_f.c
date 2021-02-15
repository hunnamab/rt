/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_f.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 10:13:46 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/09 18:24:19 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int				conversion(t_mask bits, t_buf *buf, t_printf *get)
{
	int					e;
	int					i;

	i = 0;
	if (is_this_number(bits, get))
		return (0);
	if (bits.bits.s == 1)
		get->sign = -1;
	e = bits.bits.e - 16383;
	while (i < 64)
	{
		if ((bits.bits.m >> (63 - i)) & 1)
		{
			if (e - i >= 0)
				take_to_2(buf, e - i);
			else if (e - i < 0)
				take_to_5(buf, -(e - i), -(e - i));
		}
		i++;
	}
	return (1);
}

void			init_f(t_printf *get)
{
	t_mask				bit_num;
	t_buf				buf;

	if (mem_alloc(&buf))
	{
		if (get->lfloat == 1)
			bit_num.num = (long double)va_arg(get->avs, long double);
		else
			bit_num.num = (double)va_arg(get->avs, double);
		if (!conversion(bit_num, &buf, get))
			return ;
		print_f(&buf, get);
		free_buf(&buf);
	}
}
