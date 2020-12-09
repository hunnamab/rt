/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/10 17:43:01 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:55:56 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

float		**create_matrix(int num_rows, int num_cols);
float		**matr_mul(float **matrix1, float **matrix2);
void		matr_mul_by_scalar(float **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_div_by_scalar(float **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_add_scalar(float **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_sub_scalar(float **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_add_matr(float **matrix1, \
float **matrix2, int num_rows, int num_cols);
void		matr_sub_matr(float **matrix1, \
float **matrix2, int num_rows, int num_cols);
float		matr_trace(float **matrix, int num_rows, int num_cols);
float		**matr_transpose(float **matr);
void		matr_copy(float **src_matrix, \
float **dst_matrix, int num_rows, int num_cols);
void		matr_free(float **matrix, int num_rows);
int			*matr_to_line(float **matrix, int num_rows, int num_cols);
void		matrix_identity(float **matrix, int num_rows);
void		matr_mul_void(float *matrix1, float **matrix2);

#endif
