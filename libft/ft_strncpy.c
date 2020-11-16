/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 17:08:50 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/18 17:17:24 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	while (n--)
	{
		if (src[i] != '\0')
		{
			dst[j] = src[i];
			i++;
			j++;
		}
		else
		{
			dst[j] = '\0';
			j++;
		}
	}
	return (dst);
}
