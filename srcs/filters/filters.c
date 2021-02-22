/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:44:37 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 18:43:11 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filters.h"

void	filters_init(t_filter_data *data)
{
	data->kernels = malloc(sizeof(cl_kernel) * 10);
	data->programs = malloc(sizeof(cl_program) * 10);
	fd_sepia(data);
	fd_negative(data);
	fd_gray_scale(data);
	fd_sharpen(data);
	fd_magic(data);
	fd_gauss(data);
}
