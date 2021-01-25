/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_matrix.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 22:16:28 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/05 22:19:44 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float	**get_translate_matrix(float *move)
{
	float **t_matrix;

	t_matrix = create_matrix(4, 4);
	matrix_identity(t_matrix, 4);
	t_matrix[0][3] = move[0];
	t_matrix[1][3] = move[1];
	t_matrix[2][3] = move[2];
	return (t_matrix);
}

float	**get_scale_matrix(float *scale)
{
	float **s_matrix;

	s_matrix = create_matrix(4, 4);
	s_matrix[0][0] = scale[0];
	s_matrix[1][1] = scale[1];
	s_matrix[2][2] = scale[2];
	s_matrix[3][3] = 1;
	return (s_matrix);
}

float	**rotate_x(float x)
{
	float **matrix;

	matrix = create_matrix(4, 4);
	matrix[0][0] = 1;
	matrix[0][2] = 0;
	matrix[1][1] = cosf(x);
	matrix[1][2] = -sinf(x);
	matrix[2][1] = sinf(x);
	matrix[2][2] = cosf(x);
	matrix[3][3] = 1;
	return (matrix);
}

float	**rotate_z(float z)
{
	float **matrix;

	matrix = create_matrix(4, 4);
	matrix[0][0] = cosf(z);
	matrix[0][1] = -sinf(z);
	matrix[1][0] = sinf(z);
	matrix[1][1] = cosf(z);
	matrix[2][2] = 1;
	matrix[3][3] = 1;
	return (matrix);
}

float	**rotate_y(float y)
{
	float **matrix;

	matrix = create_matrix(4, 4);
	matrix[0][0] = cosf(y);
	matrix[0][2] = sinf(y);
	matrix[1][1] = 1;
	matrix[2][0] = -sinf(y);
	matrix[2][2] = cosf(y);
	matrix[3][3] = 1;
	return (matrix);
}
