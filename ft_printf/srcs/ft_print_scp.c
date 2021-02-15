/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_scp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: poatmeal <poatmeal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 16:05:29 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/30 15:10:58 by poatmeal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void				ft_print_all(t_printf *f)
{
	if (f->star == 1)
		ft_putstr("It's bonus\n");
	else
	{
		(f->convs == 's') ? ft_print_str(f) : 0;
		(f->convs == 'c') ? ft_print_char(f) : 0;
		(f->convs == 'p') ? ft_print_address(f) : 0;
		(f->convs == 'd' || f->convs == 'i') ? ft_print_int(f) : 0;
		(f->convs == 'u' || f->convs == 'U') ? ft_print_uint(f) : 0;
		(f->convs == 'o') ? ft_print_oint(f) : 0;
		(f->convs == 'x' || f->convs == 'X') ? ft_print_xint(f) : 0;
		(f->convs == 'f') ? init_f(f) : 0;
		(f->convs == '%') ? ft_print_percent(f) : 0;
		(f->convs == 'Z') ? ft_print_other(f) : 0;
	}
}

void				ft_print_str(t_printf *f)
{
	char			*res;
	int				length;

	if (!(res = va_arg(f->avs, char*)))
		res = "(null)";
	if (f->precis >= 0)
		res = ft_strndup(res, f->precis);
	length = ft_strlen(res);
	if (f->width > 0 && !f->fm)
		(f->fz) ? ft_spacing('0', f, length) : ft_spacing(' ', f, length);
	ft_putstr(res);
	if (f->width > 0 && f->fm)
		ft_spacing(' ', f, length);
	f->len += length;
	if (f->precis >= 0)
		ft_strdel(&res);
}

void				ft_print_char(t_printf *f)
{
	int				res;
	int				length;

	if (ft_strcmp(f->modln, "l") == 0)
		res = (unsigned long)va_arg(f->avs, long int);
	else
		res = (char)va_arg(f->avs, int);
	length = 1;
	if (f->width > 0 && !f->fm)
		(f->fz) ? ft_spacing('0', f, length) : ft_spacing(' ', f, length);
	ft_putchar(res);
	if (f->width > 0 && f->fm)
		ft_spacing(' ', f, length);
	f->len += length;
}

void				ft_print_address(t_printf *f)
{
	unsigned long	res;
	int				length;
	char			*s;

	res = (unsigned long)va_arg(f->avs, void*);
	s = ftbaseull(res, 16, 'a');
	length = ft_strlen(s);
	if (f->width >= 0 && !f->fm)
	{
		if (f->precis > f->width)
		{
			ft_putstr("0x");
			ft_ispacing('0', f, length + 1);
		}
		else
		{
			ft_spacing(' ', f, length + 2);
			ft_putstr("0x");
		}
	}
	(f->fm) ? ft_putstr("0x") : 0;
	(res == 0 && f->precis == 0) ? 0 : ft_putstr(s);
	(f->width > 0 && f->fm) ? ft_spacing(' ', f, length + 2) : 0;
	(res == 0 && f->precis == 0) ? (f->len += 2) : (f->len += length + 2);
	free(s);
}

void				ft_spacing(char c, t_printf *f, int length)
{
	int				lensp;

	if ((lensp = f->width - length) > 0)
		while (lensp > 0)
		{
			ft_putchar(c);
			f->len++;
			lensp--;
		}
}
