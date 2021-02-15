/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/06 12:44:47 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/07 21:15:51 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void				*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*dst;

	i = 0;
	dst = (unsigned char *)b;
	while (i < len)
	{
		dst[i] = c;
		i++;
	}
	return (dst);
}
