/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:02:45 by pmetron           #+#    #+#             */
/*   Updated: 2020/08/07 17:36:05 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#define MAX_SIZE 80

void	norme(int i, int *num)
{
	while (i > 0)
	{
		if (num[i] > 9)
		{
			num[i - 1]++;
			num[i] = 0;
		}
		i--;
	}
}

void	rounding(long double f, int precision, int i, int *num)
{
	int	buf;
	int	dot;

	dot = i;
	while (i < MAX_SIZE)
	{
		f *= 10;
		buf = (long long int)f;
		f -= buf;
		num[i] = buf;
		i++;
	}
	i = MAX_SIZE - 1;
	while (i > precision + dot - 1)
	{
		if (num[i] > 4)
			num[i - 1]++;
		num[i] = 0;
		i--;
	}
	norme(i, num);
}

char	*int_to_string(int *num, int dot, int precision, int j)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strnew(dot + precision + 1);
	result[dot + precision] = '\0';
	while (i < dot)
	{
		if (num[i] > 9)
			result[j++] = num[i] / 10 + '0';
		result[j++] = num[i++] % 10 + '0';
	}
	if (precision)
		result[j++] = '.';
	while (i - dot < precision)
	{
		if (i >= MAX_SIZE)
		{
			result[j++] = '0';
			i++;
		}
		else
			result[j++] = num[i++] % 10 + '0';
	}
	return (result);
}

char	*ftoa(long double f, unsigned int precision)
{
	int		*num;
	char	*integer;
	int		i;
	int		dot;
	char	*result;

	i = -1;
	integer = itoa_ll((long long int)f);
	num = malloc(sizeof(int) * MAX_SIZE);
	while (integer[++i] != '\0')
		num[i] = integer[i] - '0';
	dot = i;
	f -= (int)f;
	rounding(f, precision, i, num);
	result = int_to_string(num, dot, precision, 0);
	free(integer);
	free(num);
	return (result);
}
