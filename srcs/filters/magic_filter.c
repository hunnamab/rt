/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_filter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:27:18 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:30:55 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	magic_filter(t_filter_data *data)
{
	size_t global;
	size_t local;
	cl_int err;
	cl_mem buffer;

	global = WID * HEI;
	buffer = clCreateBuffer(data->context, 0, \
					sizeof(t_color) * WID * HEI, NULL, &err);
	clEnqueueCopyBuffer(data->commands, data->pixels, \
						buffer, 0, 0, sizeof(t_color) * WID * HEI, 0, 0, 0);
	err = clSetKernelArg(data->kernels[MAGIC], 0, \
											sizeof(cl_mem), &data->pixels);
	err = clSetKernelArg(data->kernels[MAGIC], 1, sizeof(cl_mem), &buffer);
	err = clGetKernelWorkGroupInfo(data->kernels[MAGIC], \
	data->device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	err = clEnqueueNDRangeKernel(data->commands, \
			data->kernels[MAGIC], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(data->commands);
	clReleaseMemObject(buffer);
}
