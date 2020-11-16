/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:03:14 by pmetron           #+#    #+#             */
/*   Updated: 2020/07/27 18:05:12 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		get_order(int num)
{
	int i;

	i = 0;
	while (num != 0)
	{
		num /= 2;
		i++;
	}
	return (i);
}

char	*get_zero(void)
{
	char	*str;

	str = ft_strnew(1);
	str[0] = '0';
	return (str);
}

char	*itoa_bin(long long int num)
{
	int		i;
	char	*result;
	int		sign;

	if (num == 0)
	{
		return (get_zero());
	}
	sign = (num < 0 ? 1 : 0);
	i = get_order(num);
	result = ft_strnew(i);
	i -= 1;
	while (num != 0)
	{
		if (num % 2 == 0)
			result[i] = '0';
		else
			result[i] = '1';
		num /= 2;
		i--;
	}
	sign > 0 ? result = ft_strjoin("-", result) : 0;
	return (result);
}
