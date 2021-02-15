/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 15:31:19 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/10 15:31:32 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_get_length(int value, int base)
{
	int		len;

	len = (value <= 0) ? 1 : 0;
	while (value)
	{
		len++;
		value /= base;
	}
	return (len);
}

char		*ft_itoa_base(int value, int base)
{
	char	*str;
	long	nbr;
	int		len;

	if (base < 2 || base > 36)
		return (NULL);
	nbr = (value < 0) ? -(long)value : (long)value;
	len = ft_get_length(value, base);
	if (!(str = ft_strnew(len)))
		return (NULL);
	len--;
	while (nbr)
	{
		if (nbr % base < 10)
			str[len--] = (nbr % base) + '0';
		else
			str[len--] = (nbr % base) + 'a' - 10;
		nbr /= base;
	}
	(len == 0) ? str[len] = '-' : 0;
	(value == 0) ? str[len] = '0' : 0;
	return (str);
}
