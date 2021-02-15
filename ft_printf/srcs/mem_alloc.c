/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 10:08:26 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/08 15:15:11 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			free_buf(t_buf *buf)
{
	free(buf->div_num);
	free(buf->wh_num);
	free(buf->pow_2);
	free(buf->pow_5);
}

int				mem_alloc(t_buf *buf)
{
	if (!(buf->wh_num = (char *)ft_memalloc(sizeof(char) * BIG_BUFF + 1)))
		return (0);
	ft_memset(buf->wh_num, -1, BIG_BUFF);
	if (!(buf->div_num = (char *)ft_memalloc(sizeof(char) * BIG_BUFF + 1)))
	{
		free(buf->wh_num);
		return (0);
	}
	ft_memset(buf->div_num, -1, BIG_BUFF);
	if (!(buf->pow_2 = (char *)ft_memalloc(sizeof(char) * BIG_BUFF + 1)))
	{
		free(buf->wh_num);
		free(buf->div_num);
		return (0);
	}
	ft_memset(buf->pow_2, -1, BIG_BUFF);
	if (!(buf->pow_5 = (char *)ft_memalloc(sizeof(char) * BIG_BUFF + 1)))
	{
		free(buf->wh_num);
		free(buf->div_num);
		free(buf->pow_5);
		return (0);
	}
	ft_memset(buf->pow_5, -1, BIG_BUFF);
	return (1);
}
