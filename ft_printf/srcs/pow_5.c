/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pow_5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 10:35:52 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/08 15:15:18 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			count_nul(t_buf *buf, int nul)
{
	int					i;
	int					n;

	i = 0;
	n = 0;
	while (buf->pow_5[BIG_BUFF - i] != -1 && nul > 0)
	{
		i++;
		nul--;
	}
	while (nul > 0)
	{
		buf->pow_5[BIG_BUFF - i] = 0;
		i++;
		nul--;
		n++;
	}
	return (n);
}

void		plus_number_5(t_buf *buf, int nul, int j)
{
	int					i;
	long long int		x;
	long long int		coef;

	coef = 0;
	j = j + count_nul(buf, nul) - 1;
	i = 0;
	nul = 0;
	while (j >= 0)
	{
		if (buf->div_num[j] == -1)
			x = buf->pow_5[BIG_BUFF - i];
		else if (buf->pow_5[BIG_BUFF - i] == -1)
			x = buf->div_num[j];
		else
			x = buf->div_num[j] + buf->pow_5[BIG_BUFF - i];
		buf->div_num[j] = (x + coef) % 10;
		if (x + coef > 9)
			coef = 1;
		else
			coef = 0;
		i++;
		j--;
	}
}

int			fill_to_5(t_buf *buf, long long int coef, long long int x)
{
	int					y;

	y = 0;
	while (buf->pow_5[BIG_BUFF - (++y)] != -1 || coef > 0)
	{
		if (buf->pow_5[BIG_BUFF - y] != -1)
		{
			x = buf->pow_5[BIG_BUFF - y] * 5 + coef;
			buf->pow_5[BIG_BUFF - y] = x % 10;
			coef = x / 10;
		}
		else
		{
			buf->pow_5[BIG_BUFF - y] = coef;
			coef = coef / 10;
		}
	}
	return (y);
}

void		take_to_5(t_buf *buf, int pow, int nul)
{
	long long int		x;
	int					y;
	long long int		coef;

	buf->pow_5[BIG_BUFF] = 5;
	if (pow == 1)
		y = 1;
	while (pow > 1)
	{
		x = buf->pow_5[BIG_BUFF] * 5;
		buf->pow_5[BIG_BUFF] = x % 10;
		coef = x / 10;
		y = fill_to_5(buf, coef, x);
		pow--;
	}
	plus_number_5(buf, nul, y);
	ft_memset(buf->pow_5, -1, BIG_BUFF);
}
