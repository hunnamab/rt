/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_double_arr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 18:02:54 by pmetron           #+#    #+#             */
/*   Updated: 2020/07/27 18:05:12 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**get_double_arr_s(int strings, int length)
{
	char **arr;

	arr = malloc(sizeof(char *) * strings + 1);
	while (strings--)
		arr[strings] = ft_strnew(length);
	arr[strings] = NULL;
	return (arr);
}

int		**get_double_arr_d(int strings, int length)
{
	int **arr;

	arr = malloc(sizeof(int *) * strings + 1);
	while (strings--)
		arr[strings] = ft_strnew_int(length);
	arr[strings] = NULL;
	return (arr);
}

double	**get_double_arr_f(int strings, int length)
{
	double **arr;

	arr = malloc(sizeof(double *) * strings + 1);
	while (strings--)
		arr[strings] = ft_strnew_double(length);
	arr[strings] = NULL;
	return (arr);
}
