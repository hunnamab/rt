/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_matrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:43:15 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:43:45 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

double	**create_matrix(int num_rows, int num_cols)
{
	int		j;
	int		i;
	double	**matrix;

	i = 0;
	j = 0;
	matrix = (double **)malloc(sizeof(double *) * num_rows);
	while (i < num_rows)
	{
		matrix[i] = (double *)malloc(sizeof(double) * num_cols);
		while (j < num_cols)
		{
			matrix[i][j] = 0.0;
			j++;
		}
		j = 0;
		i++;
	}
	return (matrix);
}
