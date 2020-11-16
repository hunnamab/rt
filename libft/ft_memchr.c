/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 16:31:21 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/18 16:38:36 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *arr, int c, size_t n)
{
	unsigned char	*chr;
	unsigned char	s;
	size_t			i;

	i = 0;
	s = (unsigned char)c;
	chr = (unsigned char *)arr;
	while (n > 0)
	{
		if (chr[i] == s)
		{
			return (&chr[i]);
		}
		i++;
		n--;
	}
	return (NULL);
}
