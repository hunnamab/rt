/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 18:18:39 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/22 13:31:24 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		reverse(char *c)
{
	size_t	start;
	size_t	finish;
	char	s;

	start = 0;
	c[0] == '-' ? start = 1 : 0;
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

static int		dec(int n)
{
	int i;

	i = 1;
	if (n < 0)
		i++;
	while (n / 10 != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char			*get_max_int(void)
{
	char *str;

	str = ft_strnew(ft_strlen("-2147483648\0"));
	str = ft_strcpy(str, "-2147483648\0");
	return (str);
}

char			*ft_itoa(int n)
{
	size_t	i;
	char	*c;

	i = 0;
	c = NULL;
	if (n == -2147483648)
		return (get_max_int());
	if ((c = ft_strnew(dec(n))))
	{
		if (n < 0)
		{
			n = -n;
			i = 1;
			c[0] = '-';
		}
		while ((n / 10) > 0)
		{
			c[i++] = n % 10 + '0';
			n = n / 10;
		}
		c[i] = n + '0';
		reverse(c);
	}
	return (c);
}
