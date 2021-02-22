/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sharpen_filter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:31:26 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:37:13 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	sharpen_filter(t_filter_data *data)
{
	size_t	gl_loc[2];
	cl_mem	buffer;
	int		num;

	num = 0;
	gl_loc[0] = WID * HEI;
	buffer = clCreateBuffer(data->context, 0, \
								sizeof(t_color) * WID * HEI, NULL, NULL);
	clEnqueueCopyBuffer(data->commands, data->pixels, \
						buffer, 0, 0, sizeof(t_color) * WID * HEI, 0, 0, 0);
	clSetKernelArg(data->kernels[SHARPEN], 0, sizeof(cl_mem), &data->pixels);
	clSetKernelArg(data->kernels[SHARPEN], 1, sizeof(cl_mem), &buffer);
	clSetKernelArg(data->kernels[SHARPEN], 2, sizeof(cl_mem), &num);
	clGetKernelWorkGroupInfo(data->kernels[SHARPEN], data->device_id, \
	CL_KERNEL_WORK_GROUP_SIZE, sizeof(gl_loc[1]), &gl_loc[1], NULL);
	clEnqueueNDRangeKernel(data->commands, \
	data->kernels[SHARPEN], 1, NULL, &gl_loc[0], &gl_loc[1], 0, NULL, NULL);
	clFinish(data->commands);
	num++;
	clEnqueueCopyBuffer(data->commands, data->pixels, \
				buffer, 0, 0, sizeof(t_color) * WID * HEI, 0, 0, 0);
	clEnqueueNDRangeKernel(data->commands, \
	data->kernels[SHARPEN], 1, NULL, &gl_loc[0], &gl_loc[1], 0, NULL, NULL);
	clFinish(data->commands);
	clReleaseMemObject(buffer);
}
