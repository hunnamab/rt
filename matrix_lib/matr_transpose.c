/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_transpose.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:48:12 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:52:35 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

double	**matr_transpose(double **matr)
{
	int		new_rows;
	int		new_cols;
	double	**trans_matrix;
	int		i;
	int		j;

	new_rows = 4;
	new_cols = 4;
	trans_matrix = create_matrix(new_rows, new_cols);
	i = 0;
	j = 0;
	while (i < new_rows)
	{
		while (j < new_cols)
		{
			trans_matrix[i][j] = matr[j][i];
			j++;
		}
		j = 0;
		i++;
	}
	return (trans_matrix);
}
