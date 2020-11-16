/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 17:18:33 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/18 17:34:46 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dst, const char *app)
{
	size_t i;

	i = ft_strlen(dst);
	while (*app)
	{
		dst[i] = *app;
		app += 1;
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
