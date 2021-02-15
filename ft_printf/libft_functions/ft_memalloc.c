/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/16 14:48:01 by poatmeal          #+#    #+#             */
/*   Updated: 2020/08/08 15:18:43 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	*ft_memalloc(size_t size)
{
	void	*new;

	new = (void *)malloc(sizeof(void) * size);
	if (new == NULL)
		return (NULL);
	ft_memset(new, 0, size);
	return (new);
}
