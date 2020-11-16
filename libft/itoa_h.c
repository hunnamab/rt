/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_h.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:03:20 by pmetron           #+#    #+#             */
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

static int		dec(short int n)
{
	int i;

	i = 1;
	while (n / 8 != 0)
	{
		n = n / 8;
		i++;
	}
	return (i);
}

char			*itoa_h(short int n)
{
	size_t	i;
	char	*c;

	i = 0;
	c = NULL;
	if ((c = ft_strnew(dec(n))))
	{
		while ((n / 8) > 0)
		{
			c[i] = n % 8 + '0';
			n /= 8;
			i++;
		}
		c[i] = n + '0';
		reverse(c);
	}
	return (c);
}
