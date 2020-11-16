/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 17:35:49 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/18 17:37:36 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *dst, const char *app, size_t n)
{
	size_t i;

	i = ft_strlen(dst);
	while (*app && n--)
	{
		dst[i] = *app;
		app += 1;
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
