/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy_alloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:02:26 by pmetron           #+#    #+#             */
/*   Updated: 2020/07/27 18:02:28 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy_alloc(char *dst, char *src)
{
	size_t i;

	i = 0;
	dst = ft_strnew(ft_strlen(src));
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	return (dst);
}
