/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_trace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:47:30 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:47:31 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

double	matr_trace(double **matrix, int num_rows, int num_cols)
{
	int		i;
	double	result;

	i = 0;
	result = 0;
	if (num_cols != num_rows)
	{
		printf("matrix isn't square");
		return (-1);
	}
	while (i < num_rows)
	{
		result += matrix[i][i];
		i++;
	}
	return (result);
}
