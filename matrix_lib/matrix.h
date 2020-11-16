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

double		**create_matrix(int num_rows, int num_cols);
double		**matr_mul(double **matrix1, double **matrix2);
void		matr_mul_by_scalar(double **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_div_by_scalar(double **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_add_scalar(double **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_sub_scalar(double **matrix, \
int num_rows, int num_cols, int scalar);
void		matr_add_matr(double **matrix1, \
double **matrix2, int num_rows, int num_cols);
void		matr_sub_matr(double **matrix1, \
double **matrix2, int num_rows, int num_cols);
double		matr_trace(double **matrix, int num_rows, int num_cols);
double		**matr_transpose(double **matr);
void		matr_copy(double **src_matrix, \
double **dst_matrix, int num_rows, int num_cols);
void		matr_free(double **matrix, int num_rows);
int			*matr_to_line(double **matrix, int num_rows, int num_cols);
void		matrix_identity(double **matrix, int num_rows);
void		matr_mul_void(double *matrix1, double **matrix2);

#endif
