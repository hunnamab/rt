/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fresnel_coeff.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 08:18:33 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 08:20:45 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		get_fresnel_coeff(t_scene *scene)
{
	size_t	global;
	size_t	local;

	global = WID * HEI;
	clSetKernelArg(scene->cl_data.kernels[17], 0, sizeof(cl_mem), \
											&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[17], 1, sizeof(cl_mem), \
											&scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[17], 2, sizeof(cl_mem), \
										&scene->cl_data.scene.material_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[17], \
				scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
									sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
				scene->cl_data.kernels[17], 1, NULL, &global, \
									&local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}
