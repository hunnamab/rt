/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base_ll.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:32:25 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 15:32:39 by aagrivan         ###   ########.fr       */
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

static unsigned long	get_length_num(long long nbr, int base, long long num)
{
	unsigned long		l;
	int					sign;

	l = (num == 0) ? 1 : 0;
	sign = (num < 0) ? 1 : 0;
	while (nbr)
	{
		l++;
		nbr /= base;
	}
	return (l + sign);
}

char					*ft_itoa_base_ll(long long num, int base)
{
	char				*str;
	unsigned long		len;
	long long			prod;
	long long			nbr;

	if (base < 2 || base > 36)
		return (NULL);
	nbr = (num < 0) ? -num : num;
	len = get_length_num(nbr, base, num);
	if (!(str = ft_strnew(len)))
		return (NULL);
	len--;
	while (nbr)
	{
		prod = nbr % base;
		nbr /= base;
		str[len--] = fill_string(prod);
	}
	(len == 0) ? str[len] = '-' : 0;
	(num == 0) ? str[len] = '0' : 0;
	return (str);
}
