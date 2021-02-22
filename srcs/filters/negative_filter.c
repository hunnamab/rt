/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   negative_filter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:42:31 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:43:36 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	negative(t_filter_data *data)
{
	size_t global;
	size_t local;
	cl_int err;

	global = WID * HEI;
	err = clSetKernelArg(data->kernels[NEGATIVE], 0, \
									sizeof(cl_mem), &data->pixels);
	err = clGetKernelWorkGroupInfo(data->kernels[NEGATIVE], \
data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	err = clEnqueueNDRangeKernel(data->commands, \
	data->kernels[NEGATIVE], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(data->commands);
}
