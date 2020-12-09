/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_mul.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:46:35 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:51:31 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

float	get_sum(float **matrix1, float **matrix2, int i, int j)
{
	float	sum;
	int		k;

	k = 0;
	sum = 0;
	while (k < 4)
	{
		sum += matrix1[i][k] * matrix2[k][j];
		k++;
	}
	return (sum);
}

void	matr_mul_void(float *matrix1, float **matrix2)
{
	int		i;
	int		j;
	float	buf;
	float	m_buf[4];

	i = -1;
	while (i++ < 4)
		m_buf[i] = matrix1[i];
	i = 0;
	j = 0;
	buf = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			buf += matrix2[i][j] * m_buf[j];
			j++;
		}
		matrix1[i] = buf;
		buf = 0;
		j = 0;
		i++;
	}
}

float	**matr_mul(float **matrix1, float **matrix2)
{
	float	**result;
	int		result_rows;
	int		result_cols;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result_rows = 4;
	result_cols = 4;
	result = create_matrix(4, 4);
	while (i < 4)
	{
		while (j < 4)
		{
			result[i][j] = get_sum(matrix1, matrix2, i, j);
			j++;
		}
		j = 0;
		i++;
	}
	return (result);
}
