/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_other.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 18:19:23 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 18:21:58 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void				ft_print_percent(t_printf *f)
{
	int				length;

	length = 1;
	if (f->width > 0 && !f->fm)
		(f->fz) ? ft_spacing('0', f, 1) : ft_spacing(' ', f, 1);
	ft_putchar('%');
	if (f->width > 0 && f->fm)
		ft_spacing(' ', f, 1);
	f->len += length;
}

void				ft_print_other(t_printf *f)
{
	int				length;

	length = 0;
	if (f->convs == 'Z')
	{
		ft_putchar('Z');
		f->len++;
	}
	f->len += length;
}
