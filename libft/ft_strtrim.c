/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 16:58:23 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/19 18:59:54 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	counter_i(char const *s)
{
	size_t i;

	i = 0;
	while (s[i] == '\t' || s[i] == '\n' || s[i] == ' ')
	{
		i++;
		if (s[i] == '\0')
			break ;
	}
	return (i);
}

static size_t	counter_index(char const *s)
{
	size_t index;

	index = ft_strlen(s);
	while (s[index - 1] == '\t' || s[index - 1] == '\n' || s[index - 1] == ' ')
	{
		index--;
	}
	return (index);
}

char			*ft_strtrim(char const *s)
{
	char	*str;
	size_t	i;
	size_t	index;
	size_t	j;
	int		sum;

	j = 0;
	str = NULL;
	if (!s)
		return (NULL);
	i = counter_i(s);
	index = counter_index(s);
	sum = index - i;
	if (sum < 0)
		sum = 0;
	if ((str = ft_strnew(sum)))
	{
		while (i < index)
		{
			str[j++] = s[i++];
		}
	}
	return (str);
}
