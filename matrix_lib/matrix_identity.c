/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_identity.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:48:44 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:48:45 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matrix.h"

void	matrix_identity(double **matrix, int num_rows)
{
	int i;

	i = 0;
	while (i < num_rows)
	{
		matrix[i][i] = 1;
		i++;
	}
}
