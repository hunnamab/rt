/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nan_inf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 17:47:09 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/09 18:30:14 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		width_inf_nan(char *arg, t_printf *get)
{
	int i;
	int n;
	int m;

	i = 0;
	n = get->width;
	m = ft_strlen(arg);
	while (i < n - m)
	{
		ft_putchar(' ');
		get->len++;
		i++;
	}
	i = 0;
	while (i < m)
	{
		ft_putchar(arg[i]);
		get->len++;
		i++;
	}
}

void		align_inf_nan(char *arg, t_printf *get)
{
	int i;
	int n;
	int m;

	i = 0;
	n = get->width;
	m = ft_strlen(arg);
	while (i < m)
	{
		ft_putchar(arg[i]);
		get->len++;
		i++;
	}
	while (i < n)
	{
		ft_putchar(' ');
		get->len++;
		i++;
	}
}

void		print_inf_nan(char *arg, t_printf *get)
{
	int len;
	int i;

	i = 0;
	len = ft_strlen(arg);
	if (get->fs && arg[0] != '+' && arg[0] != '-' && arg[0] != 'n')
	{
		ft_putchar(' ');
		get->len++;
	}
	if (get->fm)
		align_inf_nan(arg, get);
	else if (get->width > 0 && len < get->width)
		width_inf_nan(arg, get);
	else
	{
		while (i < len)
		{
			ft_putchar(arg[i]);
			get->len++;
			i++;
		}
	}
}

int			is_this_number(t_mask bits, t_printf *get)
{
	if (bits.bits.e == 32767)
	{
		if (bits.bits.m == 0x8000000000000000 && bits.bits.s == 0 && get->fp)
			print_inf_nan("+inf", get);
		else if (bits.bits.m == 0x8000000000000000 && bits.bits.s == 0 &&
		!get->fp)
			print_inf_nan("inf", get);
		else if (bits.bits.m == 0x8000000000000000 && bits.bits.s == 1)
			print_inf_nan("-inf", get);
		else
			print_inf_nan("nan", get);
		return (1);
	}
	return (0);
}
