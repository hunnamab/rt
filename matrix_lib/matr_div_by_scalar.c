/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_div_by_scalar.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:44:30 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:49:25 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

void	matr_div_by_scalar(double **matrix, int num_rows,\
int num_cols, int scalar)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < num_rows)
	{
		while (j < num_cols)
		{
			matrix[i][j] /= scalar;
			j++;
		}
		j = 0;
		i++;
	}
}
