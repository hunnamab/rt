/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_di.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 19:26:59 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 17:35:55 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void				prec_big_width(t_printf *f, long long res, int length)
{
	(res < 0 || f->fp) ? ft_ispacing(' ', f, f->precis + 1)\
	: ft_ispacing(' ', f, f->precis);
	if (res < 0 || f->fp)
	{
		(res >= 0 && f->fp) ? ft_putchar('+') : ft_putchar('-');
		f->len++;
	}
	ft_ispacing('0', f, (f->width - f->precis + length));
}

static void				pr_fl_with_wi(t_printf *f, long long res, int length)
{
	if (f->fz && f->precis < 0)
	{
		print_sign(f, res, length);
		ft_ispacing('0', f, (length + 1));
	}
	else if (f->precis >= f->width)
	{
		print_sign(f, res, length);
		ft_ispacing('0', f, (length + 1));
	}
	else if (f->precis <= length)
	{
		if (res < 0 || f->fp)
		{
			ft_ispacing(' ', f, length + 1);
			(res >= 0 && f->fp) ? ft_putchar('+') : ft_putchar('-');
			f->len++;
		}
		else
			(res == 0 && f->precis == 0) ? ft_ispacing(' ', f, 0)\
			: ft_ispacing(' ', f, length);
	}
	else if (f->precis < f->width)
		prec_big_width(f, res, length);
}

static void				pr_fl_out_wi(t_printf *f, long long res, int length)
{
	if (res < 0)
	{
		ft_putchar('-');
		f->len++;
	}
	if (res >= 0 && (f->fp || f->fs))
	{
		(f->fp && res >= 0) ? ft_putchar('+') : 0;
		(f->fs && !f->fp && res > 0) ? ft_putchar(' ') : 0;
		f->len++;
	}
	if (f->precis >= f->width)
		ft_ispacing('0', f, (length + 1));
	else
		(f->fz && f->fp && f->fs && f->fm && f->precis > 0) ? 0\
		: ft_ispacing('0', f, (((f->width - f->precis) + length)));
}

static void				width_fm_print(t_printf *f, long long res, int length)
{
	if (f->precis < f->width)
	{
		if (f->precis > length)
		{
			if (f->fz && f->fp && f->fs && f->fm && f->precis > 0)
				ft_ispacing(' ', f, length + 1);
			else if (f->fp || f->fs || res < 0)
				ft_ispacing(' ', f, f->precis + 1);
			else
				ft_ispacing(' ', f, f->precis);
		}
		else
		{
			if (f->fp || f->fs || res < 0)
				ft_ispacing(' ', f, length + 1);
			else if (res == 0 && f->precis >= 0)
				ft_ispacing(' ', f, 0);
			else
				ft_ispacing(' ', f, length);
		}
	}
}

void					ft_print_int(t_printf *f)
{
	intmax_t			res;
	int					length;
	char				*s;
	char				zero;
	unsigned long long	res_c;

	zero = '0';
	if (!(res = ft_get_num_modlen(f)))
		zero = '1';
	if (res < -9223372036854775807)
	{
		res_c = (unsigned long long)LLONG_MAX + 1;
		s = ftbaseull(res_c, 10, 'a');
	}
	else
		s = ft_itoa_base_ll_pos(res, 10);
	length = ft_strlen(s);
	if (f->width > 0 && !f->fm)
		pr_fl_with_wi(f, res, length);
	else if (f->width == 0 || (f->width > 0 && f->fm))
		pr_fl_out_wi(f, res, length);
	(f->precis == 0 && zero == '1') ? 0 : ft_putstr(s);
	(f->width > 0 && f->fm) ? width_fm_print(f, res, length) : 0;
	(f->precis == 0 && zero == '1') ? (f->len += 0) : (f->len += length);
	free(s);
}
