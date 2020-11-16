/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew_double.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:02:36 by pmetron           #+#    #+#             */
/*   Updated: 2020/07/27 18:05:12 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	*ft_strnew_double(size_t size)
{
	double *mem;

	if (NULL == (mem = malloc(sizeof(double) * (size))))
		return (NULL);
	ft_memset((void *)mem, 0, 8 * size);
	return (mem);
}
