/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gray_scale_filter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:44:18 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:45:00 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	gray_scale(t_filter_data *data)
{
	size_t global;
	size_t local;
	cl_int err;

	global = WID * HEI;
	err = clSetKernelArg(data->kernels[GRAYSCALE], 0, \
										sizeof(cl_mem), &data->pixels);
	err = clGetKernelWorkGroupInfo(data->kernels[GRAYSCALE], \
	data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	err = clEnqueueNDRangeKernel(data->commands, \
	data->kernels[GRAYSCALE], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(data->commands);
}
