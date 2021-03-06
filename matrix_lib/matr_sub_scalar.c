/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_sub_scalar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:46:52 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:46:56 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

void	matr_sub_scalar(float **matrix, int num_rows, int num_cols, int scalar)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < num_rows)
	{
		while (j < num_cols)
		{
			matrix[i][j] -= scalar;
			j++;
		}
		j = 0;
		i++;
	}
}
