/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_to_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:47:09 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:47:10 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

int		*matr_to_line(double **matrix, int num_rows, int num_cols)
{
	int *line;
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	line = (int *)malloc(sizeof(int) * num_rows * num_cols);
	while (i < num_rows)
	{
		while (j < num_cols)
		{
			line[k] = matrix[i][j];
			j++;
			k++;
		}
		j = 0;
		i++;
	}
	return (line);
}
