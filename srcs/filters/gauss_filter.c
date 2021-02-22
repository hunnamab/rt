/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gauss_filter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:38:12 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:41:47 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	gauss_filter(t_filter_data *data)
{
	size_t global;
	size_t local;
	cl_mem buffer;
	cl_int type;

	type = 0;
	global = WID * HEI;
	buffer = clCreateBuffer(data->context, 0, \
								sizeof(t_color) * WID * HEI, NULL, NULL);
	clEnqueueCopyBuffer(data->commands, data->pixels, buffer, 0, 0, \
								sizeof(t_color) * WID * HEI, 0, 0, 0);
	clSetKernelArg(data->kernels[GAUSS], 0, sizeof(cl_mem), &data->pixels);
	clSetKernelArg(data->kernels[GAUSS], 1, sizeof(cl_mem), &buffer);
	clSetKernelArg(data->kernels[GAUSS], 2, sizeof(cl_int), &type);
	clGetKernelWorkGroupInfo(data->kernels[GAUSS], data->device_id, \
					CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(data->commands, data->kernels[GAUSS], 1, \
					NULL, &global, &local, 0, NULL, NULL);
	clFinish(data->commands);
	clEnqueueCopyBuffer(data->commands, data->pixels, buffer, 0, 0, \
							sizeof(t_color) * WID * HEI, 0, 0, 0);
	clEnqueueNDRangeKernel(data->commands, data->kernels[GAUSS], 1, \
								NULL, &global, &local, 0, NULL, NULL);
	clFinish(data->commands);
	clReleaseMemObject(buffer);
}
