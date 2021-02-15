/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/12 18:02:10 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/07 21:16:40 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		*ft_strnew(size_t size)
{
	char	*mem;
	size_t	i;

	i = 0;
	if (!(mem = (char *)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	while (size > 0)
	{
		mem[i] = '\0';
		size--;
		i++;
	}
	mem[i] = '\0';
	return (mem);
}
