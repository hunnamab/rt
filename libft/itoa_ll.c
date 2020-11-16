/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_ll.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:04:25 by pmetron           #+#    #+#             */
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

static int		dec(long long int n)
{
	int i;

	i = 1;
	while (n / 10 != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char			*itoa_ll(long long int n)
{
	size_t	i;
	char	*c;

	i = 0;
	c = NULL;
	if ((c = ft_strnew(dec(n))))
	{
		while ((n / 10) > 0)
		{
			c[i] = n % 10 + '0';
			n /= 10;
			i++;
		}
		c[i] = n + '0';
		reverse(c);
	}
	return (c);
}
