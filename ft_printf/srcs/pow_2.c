/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pow_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 10:35:00 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/08 15:15:14 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		plus_number_2(t_buf *buf)
{
	size_t				i;
	long long int		x;
	long long int		coef;

	i = 0;
	coef = 0;
	while (buf->wh_num[BIG_BUFF - i] != -1 ||
	buf->pow_2[BIG_BUFF - i] != -1 || coef > 0)
	{
		if (buf->wh_num[BIG_BUFF - i] == -1 && buf->pow_2[BIG_BUFF - i] == -1)
			x = 0;
		else if (buf->wh_num[BIG_BUFF - i] == -1)
			x = buf->pow_2[BIG_BUFF - i];
		else if (buf->pow_2[BIG_BUFF - i] == -1)
			x = buf->wh_num[BIG_BUFF - i];
		else
			x = buf->wh_num[BIG_BUFF - i] + buf->pow_2[BIG_BUFF - i];
		buf->wh_num[BIG_BUFF - i] = (x + coef) % 10;
		if (x + coef > 9)
			coef = 1;
		else
			coef = 0;
		i++;
	}
}

void		fill_to_2(t_buf *buf, long long int coef, long long int x)
{
	int					y;

	y = 0;
	while (buf->pow_2[BIG_BUFF - (++y)] != -1 || coef > 0)
	{
		if (buf->pow_2[BIG_BUFF - y] != -1)
		{
			x = buf->pow_2[BIG_BUFF - y] * 2 + coef;
			buf->pow_2[BIG_BUFF - y] = x % 10;
			coef = x / 10;
		}
		else
		{
			buf->pow_2[BIG_BUFF - y] = coef;
			coef = coef / 10;
		}
	}
}

void		take_to_2(t_buf *buf, int pw)
{
	long long int		x;
	long long int		coef;

	if (pw == 0)
		buf->pow_2[BIG_BUFF] = 1;
	else
		buf->pow_2[BIG_BUFF] = 2;
	while (pw > 1)
	{
		x = buf->pow_2[BIG_BUFF] * 2;
		buf->pow_2[BIG_BUFF] = x % 10;
		coef = x / 10;
		fill_to_2(buf, coef, x);
		pw--;
	}
	plus_number_2(buf);
	ft_memset(buf->pow_2, -1, BIG_BUFF);
}
