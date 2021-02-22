/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sepia_filter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:46:18 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:46:47 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	sepia_filter(t_filter_data *data)
{
	size_t global;
	size_t local;
	cl_int err;

	global = WID * HEI;
	err = clSetKernelArg(data->kernels[0], 0, sizeof(cl_mem), &data->pixels);
	err = clGetKernelWorkGroupInfo(data->kernels[0], \
	data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	err = clEnqueueNDRangeKernel(data->commands, \
	data->kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(data->commands);
}
