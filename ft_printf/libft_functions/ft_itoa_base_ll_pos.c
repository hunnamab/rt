/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base_ll_pos.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:33:44 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 15:35:04 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char				fill_string(long long prod)
{
	char				c;

	c = '0';
	if (prod < 10)
		c = prod + '0';
	else if (prod >= 10)
		c = prod + 'a' - 10;
	return (c);
}

static unsigned long	get_length_num(long long nbr, int base)
{
	unsigned long		l;

	l = (nbr == 0) ? 1 : 0;
	while (nbr)
	{
		l++;
		nbr /= base;
	}
	return (l);
}

char					*ft_itoa_base_ll_pos(long long num, int base)
{
	char				*str;
	unsigned long		len;
	long long			prod;
	long long			nbr;

	if (base < 2 || base > 36)
		return (NULL);
	nbr = (num < 0) ? -num : num;
	len = get_length_num(nbr, base);
	if (!(str = ft_strnew(len)))
		return (NULL);
	len--;
	while (nbr)
	{
		prod = nbr % base;
		nbr /= base;
		str[len--] = fill_string(prod);
	}
	(num == 0) ? str[len] = '0' : 0;
	return (str);
}
