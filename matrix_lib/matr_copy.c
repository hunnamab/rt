/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_copy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:44:22 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:53:38 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

void	matr_copy(double **src_matrix, double **dst_matrix, \
int num_rows, int num_cols)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (i < num_rows)
	{
		while (j < num_cols)
		{
			dst_matrix[i][j] = src_matrix[i][j];
			j++;
		}
		j = 0;
		i++;
	}
}
