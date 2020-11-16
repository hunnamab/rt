/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 17:40:49 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/20 19:09:50 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_strlcat(char *s1, const char *s2, size_t n)
{
	size_t i;
	size_t j;
	size_t result;

	i = 0;
	while (s1[i] != '\0')
		++i;
	result = 0;
	while (s2[result] != '\0')
		++result;
	if (n <= i)
		result += n;
	else
		result += i;
	j = 0;
	while (s2[j] != '\0' && i + 1 < n)
	{
		s1[i] = s2[j];
		i++;
		j++;
	}
	s1[i] = '\0';
	return (result);
}
