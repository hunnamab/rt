/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   round.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/08 14:25:21 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/08 17:25:20 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			check_round(t_buf *buf, int i)
{
	while (i < BIG_BUFF && buf->div_num[i] == 0)
		i++;
	if (i < BIG_BUFF && buf->div_num[i] != -1)
		change_whole(buf);
}

void			check_round_div(t_buf *buf, int i)
{
	int j;

	j = i;
	while (i < BIG_BUFF && buf->div_num[i] == 0)
		i++;
	if (i < BIG_BUFF && buf->div_num[i] != -1)
		change_div(buf, j - 2);
}

void			round_whole(t_buf *buf, int i)
{
	if (buf->div_num[i] > 5)
		change_whole(buf);
	else if (buf->div_num[i] == 5 && buf->div_num[i + 1] == -1)
	{
		if (buf->wh_num[BIG_BUFF] % 2 != 0)
			change_whole(buf);
	}
	else if (buf->div_num[i] == 5 && buf->div_num[i + 1] != -1)
		check_round(buf, i + 1);
}

void			round_div(t_buf *buf, int i)
{
	if (buf->div_num[i] > 5)
		change_div(buf, i - 1);
	else if (buf->div_num[i] == 5 && buf->div_num[i + 1] == -1)
	{
		if (buf->div_num[i - 1] % 2 != 0)
			change_div(buf, i - 1);
	}
	else if (buf->div_num[i] == 5 && buf->div_num[i + 1] != -1)
		check_round_div(buf, i + 1);
}

void			ft_round(t_buf *buf, t_printf *get)
{
	int i;
	int j;

	j = 0;
	i = get->f_prec;
	if (i == 0)
		round_whole(buf, i);
	else
		round_div(buf, i);
}
