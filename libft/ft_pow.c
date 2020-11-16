/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:02:18 by pmetron           #+#    #+#             */
/*   Updated: 2020/07/27 18:02:19 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long int	ft_pow(long long int num, long long int pow)
{
	int buf;
	int i;

	buf = num;
	i = 0;
	while (i < pow - 1)
	{
		num *= buf;
		i++;
	}
	return (num);
}
