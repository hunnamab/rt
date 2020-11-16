/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 16:39:29 by pmetron           #+#    #+#             */
/*   Updated: 2019/09/18 16:51:14 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *arr1, const void *arr2, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;

	a = (unsigned char *)arr1;
	b = (unsigned char *)arr2;
	while (n > 0)
	{
		if (*a != *b)
			return (*a - *b);
		a += 1;
		b += 1;
		n--;
	}
	return (0);
}
