/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:34:00 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 13:27:36 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double	ftoi(char *str)
{
	size_t	i;
	double	n;
	double	sign;
	double	buf;

	n = 0;
	buf = 1;
	sign = str[0] == '-' ? -1 : 1;
	i = sign == 1 ? 0 : 1;
	while (str[i] != '\0' && str[i] != '.')
	{
		n *= 10;
		n += (str[i] - '0');
		i++;
	}
	str[i] != '\0' ? i += 1 : 0;
	while (str[i] != '\0')
	{
		n += (str[i] - '0') / (10 * buf);
		buf *= 10;
		i++;
	}
	ft_memdel((void **)&str);
	return (n * sign);
}
