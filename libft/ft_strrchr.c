/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 18:59:43 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/18 19:03:31 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int ch)
{
	int		i;
	char	*s;

	s = (char *)str;
	i = ft_strlen(s);
	if (ch == '\0' && s[i + 1] == '\0')
		return (&s[i + 1]);
	while (i >= 0)
	{
		if (s[i] == ch)
		{
			return (&s[i]);
		}
		i--;
	}
	return (NULL);
}
