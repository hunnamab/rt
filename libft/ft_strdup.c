/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 16:54:26 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/19 16:25:33 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*new;
	size_t	i;

	new = NULL;
	i = 0;
	if ((new = ft_strnew(ft_strlen(str))))
	{
		while (str[i] != '\0')
		{
			new[i] = str[i];
			i++;
		}
		new[i] = '\0';
	}
	return (new);
}
