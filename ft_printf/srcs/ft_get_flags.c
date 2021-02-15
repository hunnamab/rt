/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/09 19:23:04 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 10:30:39 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static t_printf		*check_flags_hzmps(t_printf *f)
{
	if (f->cpy[f->i] == '#')
		f->fh = '#';
	else if (f->cpy[f->i] == '0')
		f->fz = '0';
	else if (f->cpy[f->i] == '-')
		f->fm = '-';
	else if (f->cpy[f->i] == '+')
		f->fp = '+';
	else if (f->cpy[f->i] == ' ')
		f->fs = ' ';
	return (f);
}

t_printf			*get_flags_hzmps(t_printf *f)
{
	t_count			pl;

	pl.k = 0;
	pl.l = 0;
	while (FLAGS[pl.l])
	{
		while (FLAGS[pl.k])
		{
			while (FLAGS[pl.k] == f->cpy[f->i])
			{
				check_flags_hzmps(f);
				f->i++;
			}
			pl.k++;
		}
		pl.k = 0;
		pl.l++;
	}
	return (f);
}

t_printf			*get_width(t_printf *f)
{
	while ((f->cpy[f->i] >= '0' && f->cpy[f->i] <= '9') || f->cpy[f->i] == '*')
	{
		if (f->cpy[f->i] == '*')
			f->star = 1;
		f->width = f->width * 10;
		f->width = f->width + (f->cpy[f->i] - '0');
		f->i++;
	}
	return (f);
}

t_printf			*get_precision(t_printf *f)
{
	f->i++;
	f->precis = 0;
	f->f_prec = 0;
	while (f->cpy[f->i] == '.')
		f->i++;
	while ((f->cpy[f->i] >= '0' && f->cpy[f->i] <= '9') || f->cpy[f->i] == '*')
	{
		if (f->cpy[f->i] == '*')
			f->star = 1;
		f->precis = f->precis * 10;
		f->precis = f->precis + (f->cpy[f->i] - '0');
		f->f_prec = f->f_prec * 10;
		f->f_prec = f->f_prec + (f->cpy[f->i] - '0');
		f->i++;
	}
	return (f);
}

t_printf			*get_mod_length(t_printf *f)
{
	t_count			m;
	char			*lh;

	m.j = 0;
	m.k = 0;
	m.l = 0;
	lh = "lhLjz";
	while (lh[m.l++])
	{
		while (lh[m.k])
		{
			if (lh[m.k] == f->cpy[f->i])
			{
				if (f->cpy[f->i] == 'L')
					f->lfloat = 1;
				f->modln[m.j++] = f->cpy[f->i++];
			}
			m.k++;
		}
		m.k = 0;
	}
	return (f);
}
