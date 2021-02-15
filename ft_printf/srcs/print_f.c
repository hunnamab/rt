/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_f.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 10:10:36 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/10 16:33:14 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			print_div(t_buf *buf, t_printf *get)
{
	int		i;

	ft_putchar('.');
	get->len++;
	i = 0;
	while (buf->div_num[i] != -1 && i < BIG_BUFF + 1 && i < get->f_prec)
	{
		ft_putchar(buf->div_num[i] + 48);
		i++;
	}
	while (i < get->f_prec)
	{
		ft_putchar('0');
		i++;
	}
	get->len += i;
}

void			print(t_buf *buf, t_printf *get)
{
	int		i;

	i = 0;
	if (get->sign == -1 && (get->width == 0 || get->fz))
	{
		ft_putchar('-');
		get->len++;
	}
	while (buf->wh_num[i] == -1)
		i++;
	while (i < BIG_BUFF + 1)
	{
		ft_putchar(buf->wh_num[i] + 48);
		get->len++;
		i++;
	}
	if (get->f_prec != 0)
		print_div(buf, get);
	if (get->f_prec == 0 && get->fh)
	{
		ft_putchar('.');
		get->len++;
	}
}

void			print_align(t_buf *buf, t_printf *get, int k, int i)
{
	int n;

	n = get->width - get->f_prec;
	if (get->f_prec != 0 || (get->f_prec == 0 && get->fh))
		n--;
	if (get->width == 0)
	{
		check_plus_space(get);
		print(buf, get);
		return ;
	}
	if (get->width < get->f_prec)
		k = 0;
	while (buf->wh_num[BIG_BUFF - i] != -1)
		i++;
	if (check_plus_space(get))
		n--;
	print(buf, get);
	while (i < n && k)
	{
		ft_putchar(' ');
		get->len++;
		i++;
	}
}

void			print_f(t_buf *buf, t_printf *get)
{
	int k;

	k = 1;
	if (get->f_prec >= 0)
		ft_round(buf, get);
	if (get->fm)
	{
		print_align(buf, get, k, 0);
		return ;
	}
	if (get->width > 0)
		fill_width(buf, get);
	else if (!get->fp && get->fs && get->sign > 0)
	{
		ft_putchar(' ');
		get->len++;
	}
	if ((get->fp && get->width <= 0) || (get->fp && get->fz))
		if (get->sign > 0)
		{
			ft_putchar('+');
			get->len++;
		}
	print(buf, get);
}
