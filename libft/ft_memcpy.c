/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 19:16:56 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/19 16:03:43 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;

	b = (unsigned char *)src;
	a = (unsigned char *)dst;
	if (!dst && !src && n)
	{
		return (NULL);
	}
	while (n)
	{
		*a++ = *b++;
		n--;
	}
	return (dst);
}
