/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aagrivan <aagrivan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/05 17:30:43 by aagrivan          #+#    #+#             */
/*   Updated: 2020/08/07 21:16:11 by aagrivan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		ft_putstr(char const *s)
{
	if (s)
		while (*s)
			ft_putchar(*s++);
}
