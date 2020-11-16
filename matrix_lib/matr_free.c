/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matr_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:44:39 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:44:40 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

void	matr_free(double **matrix, int num_rows)
{
	int i;

	i = 0;
	while (i < num_rows)
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}
