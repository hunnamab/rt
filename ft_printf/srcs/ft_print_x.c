/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_x.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 16:11:26 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 18:53:53 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int				case_zero(t_printf *f, int res)
{
	if (f->fh && res != 0)
	{
		ft_putchar('0');
		ft_putchar(f->convs);
		f->len += 2;
	}
	return (f->len);
}

static void				x_print_fl_with_fm(t_printf *f, int length, int res)
{
	if (f->fz && f->precis < 0)
	{
		case_zero(f, res);
		(f->fh && res != 0) ? ft_ispacing('0', f, length + 2)\
		: ft_ispacing('0', f, length);
	}
	else if (f->precis >= f->width)
	{
		case_zero(f, res);
		ft_ispacing('0', f, (length + 1));
	}
	else if (f->precis <= length)
	{
		(f->fh && res != 0) ? ft_ispacing(' ', f, (length + 2))\
		: ft_ispacing(' ', f, length);
		case_zero(f, res);
		(!f->fh && res == 0) ? ft_ispacing(' ', f, length + 3) : 0;
	}
	else if (f->precis < f->width)
	{
		(f->fh && res != 0) ? ft_ispacing(' ', f, f->precis + 2)\
		: ft_ispacing(' ', f, f->precis);
		case_zero(f, res);
		ft_ispacing('0', f, (f->width - f->precis + length));
	}
}

static void				x_pr_fl_with_fm(t_printf *f, int length, int res)
{
	if (f->precis >= f->width)
	{
		case_zero(f, res);
		ft_ispacing('0', f, length + 1);
	}
	else if (f->precis <= length)
	{
		case_zero(f, res);
	}
	else if (f->precis < f->width)
	{
		case_zero(f, res);
		ft_ispacing('0', f, (f->width - f->precis + length));
	}
}

static void				width_fm(t_printf *f, int length, int res)
{
	if (f->precis <= length)
	{
		if (res == 0 && f->precis == 0)
			ft_ispacing(' ', f, 0);
		else
			(f->fh && res != 0) ? ft_ispacing(' ', f, (length + 2))\
			: ft_ispacing(' ', f, length);
	}
	else if (f->precis < f->width)
		(f->fh && res != 0) ? ft_ispacing(' ', f, f->precis + 2)\
		: ft_ispacing(' ', f, f->precis);
}

void					ft_print_xint(t_printf *f)
{
	uintmax_t			res;
	unsigned int		length;
	char				*s;

	res = ft_get_unum_modlen(f);
	s = (f->convs == 'x') ? ftbaseull(res, 16, 'a') : \
	ftbaseull(res, 16, 'A');
	length = ft_strlen(s);
	if (f->width >= 0 && !f->fm)
		x_print_fl_with_fm(f, length, res);
	if (f->width >= 0 && f->fm)
		x_pr_fl_with_fm(f, length, res);
	(f->precis == 0 && res == 0) ? 0 : ft_putstr(s);
	if (f->width > 0 && f->fm)
		width_fm(f, length, res);
	(f->precis == 0 && res == 0) ? (f->len += 0)\
	: (f->len += length);
	free(s);
}
