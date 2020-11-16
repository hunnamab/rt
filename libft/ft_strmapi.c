/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 13:50:49 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/22 18:10:03 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*buf;
	unsigned int	i;
	unsigned int	j;

	buf = NULL;
	if (s && f)
	{
		if ((buf = ft_strnew(ft_strlen(s))))
		{
			i = 0;
			j = ft_strlen(s);
			while (i < j)
			{
				buf[i] = f(i, s[i]);
				i++;
			}
			buf[i] = '\0';
		}
	}
	return (buf);
}
