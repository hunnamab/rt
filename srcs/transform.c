/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 22:17:09 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/09 12:42:41 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

float	*get_three_floats(float x, float y, float z)
{
	float	*angles;

	angles = protected_malloc(sizeof(float), 3);
	angles[0] = x;
	angles[1] = y;
	angles[2] = z;
	return (angles);
}

float	**get_rotation_matrix_euler(float *angle)
{
	float **rotation_matrix;
	float x;
	float y;
	float z;

	x = angle[0];
	y = angle[1];
	z = angle[2];
	rotation_matrix = create_matrix(4, 4);
	rotation_matrix[0][0] = cosf(x) * cosf(z) - cosf(y) * sinf(x) * sinf(z);
	rotation_matrix[1][0] = cosf(y) * cosf(z) * sinf(x) + cosf(x) * sinf(z);
	rotation_matrix[2][0] = sinf(x) * sinf(y);
	rotation_matrix[0][1] = -cosf(z) * sinf(x) - cosf(x) * cosf(y) * sinf(z);
	rotation_matrix[1][1] = cosf(x) * cosf(y) * cosf(z) - sinf(x) * sinf(z);
	rotation_matrix[2][1] = cosf(x) * sinf(y);
	rotation_matrix[0][2] = sinf(y) * sinf(z);
	rotation_matrix[1][2] = -cosf(z) * sinf(y);
	rotation_matrix[2][2] = cosf(y);
	rotation_matrix[3][3] = 1;
	return (rotation_matrix);
}

float	**get_rotation_matrix(float *angle)
{
	float **rotation_matrix;
	float **rotation_buf;
	float **rotation_buf_second;

	rotation_buf = rotate_z(angle[2]);
	rotation_buf_second = rotate_y(angle[1]);
	rotation_matrix = matr_mul(rotation_buf, rotation_buf_second);
	matr_free(rotation_buf, 4);
	matr_free(rotation_buf_second, 4);
	rotation_buf = rotate_x(angle[0]);
	rotation_buf_second = matr_mul(rotation_matrix, rotation_buf);
	matr_free(rotation_buf, 4);
	matr_free(rotation_matrix, 4);
	return (rotation_buf_second);
}

void	transform(cl_float3 *point, float **matrix, int point_nmb)
{
	int		i;
	float	coord_matrix[4];

	i = 0;
	while (i < point_nmb)
	{
		coord_matrix[0] = point[i].x;
		coord_matrix[1] = point[i].y;
		coord_matrix[2] = point[i].z;
		coord_matrix[3] = 1;
		matr_mul_void(coord_matrix, matrix);
		point[i].x = coord_matrix[0];
		point[i].y = coord_matrix[1];
		point[i].z = coord_matrix[2];
		i++;
	}
}

float	**get_transform_matrix(float *angle, float *move, float *scale)
{
	float **t_matrix;
	float **s_matrix;
	float **rotation_matrix;
	float **result_matrix;
	float **result;

	t_matrix = get_translate_matrix(move);
	s_matrix = get_scale_matrix(scale);
	rotation_matrix = get_rotation_matrix(angle);
	result_matrix = matr_mul(s_matrix, rotation_matrix);
	matr_free(rotation_matrix, 4);
	matr_free(s_matrix, 4);
	result = matr_mul(t_matrix, result_matrix);
	matr_free(result_matrix, 4);
	matr_free(t_matrix, 4);
	free(angle);
	free(move);
	free(scale);
	return (result);
}
