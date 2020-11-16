/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 14:24:58 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/19 16:24:36 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;
	unsigned char	s;
	size_t			i;

	i = 0;
	s = c;
	b = (unsigned char *)src;
	a = (unsigned char *)dst;
	while (i < n)
	{
		a[i] = b[i];
		if (b[i] == s)
			return (dst + i + 1);
		i++;
	}
	return (NULL);
}
