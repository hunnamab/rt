/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 16:15:59 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 19:46:51 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int				ft_printf(const char *format, ...)
{
	t_printf	*f;
	int			length;

	length = 0;
	if (!(f = ft_init(format)))
		return (-1);
	if (format)
	{
		va_start(f->avs, format);
		length = ft_parse_format(f);
		va_end(f->avs);
	}
	free(f);
	return (length);
}

t_printf		*ft_init(const char *format)
{
	t_printf	*new;

	if (!(new = (t_printf*)malloc(sizeof(t_printf))))
		return (0);
	new->cpy = format;
	new->width = 0;
	new->precis = -1;
	new->f_prec = 6;
	new->star = 0;
	new->fh = '\0';
	new->fz = '\0';
	new->fm = '\0';
	new->fp = '\0';
	new->fs = '\0';
	new->sign = 1;
	new->lfloat = 0;
	ft_memset(new->modln, '\0', 4);
	new->convs = '\0';
	new->len = 0;
	new->i = 0;
	return (new);
}

int				ft_parse_format(t_printf *f)
{
	while (f->cpy[f->i])
	{
		if (f->cpy[f->i] == '%')
		{
			if (!(ft_strchr(NEEDED, f->cpy[f->i + 1])))
			{
				f->i++;
				ft_putchar(f->cpy[f->i]);
				f->len++;
			}
			else
			{
				if (!(ft_get_all_flags(f)))
					return (f->len);
			}
			ft_reset_init(f);
		}
		else
		{
			ft_putchar(f->cpy[f->i]);
			f->len++;
		}
		f->i++;
	}
	return (f->len);
}

t_printf		*ft_reset_init(t_printf *f)
{
	f->width = 0;
	f->precis = -1;
	f->f_prec = 6;
	f->star = 0;
	f->fh = '\0';
	f->fz = '\0';
	f->fm = '\0';
	f->fp = '\0';
	f->fs = '\0';
	f->sign = 1;
	f->lfloat = 0;
	ft_memset(f->modln, '\0', 4);
	f->convs = '\0';
	return (f);
}

int				ft_get_all_flags(t_printf *f)
{
	f->i++;
	if (f->cpy[f->i] == '\0')
		return (0);
	get_flags_hzmps(f);
	get_width(f);
	if (f->cpy[f->i] == '.')
		get_precision(f);
	get_mod_length(f);
	get_conversion(f);
	if (f->convs)
		ft_print_all(f);
	else
		return (0);
	return (1);
}
