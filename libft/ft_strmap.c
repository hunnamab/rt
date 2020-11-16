/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/09 19:37:36 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/22 18:09:30 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*buf;
	int		i;

	i = 0;
	buf = NULL;
	if (s && f)
	{
		if ((buf = ft_memalloc(ft_strlen(s) + 1)))
		{
			while (s[i] != '\0')
			{
				buf[i] = f(s[i]);
				i++;
			}
			buf[i + 1] = '\0';
		}
	}
	return (buf);
}
