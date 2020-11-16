/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_hex_ll.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:04:00 by pmetron           #+#    #+#             */
/*   Updated: 2020/07/27 18:05:12 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		reverse(char *c)
{
	size_t	start;
	size_t	finish;
	char	s;

	start = 0;
	finish = ft_strlen(c) - 1;
	while (start < finish)
	{
		s = c[start];
		c[start] = c[finish];
		c[finish] = s;
		start++;
		finish--;
	}
}

static int		hex(unsigned long long int n)
{
	int i;

	i = 1;
	while (n / 16 != 0)
	{
		n = n / 16;
		i++;
	}
	return (i);
}

char			*itoa_hex_ll(unsigned long long int n)
{
	size_t	i;
	char	*c;

	i = 0;
	c = NULL;
	if ((c = ft_strnew(hex(n))))
	{
		while ((n / 16) > 0)
		{
			if (n % 16 < 10)
				c[i] = n % 16 + '0';
			else
				c[i] = 'A' + (n % 16) % 10;
			n = n / 16;
			i++;
		}
		if (n < 10)
			c[i] = n + '0';
		else
			c[i] = 'A' + (n % 16) % 10;
		reverse(c);
	}
	return (c);
}
