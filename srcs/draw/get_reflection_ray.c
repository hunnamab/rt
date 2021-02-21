/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_reflection_ray.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 08:33:38 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/22 02:53:52 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	set_arg(t_scene *scene, size_t local)
{
	clSetKernelArg(scene->cl_data.kernels[16], 0, sizeof(cl_mem), \
										&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[16], 1, sizeof(cl_mem), \
										&scene->cl_data.scene.normal_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[16], \
					scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
								sizeof(local), &local, NULL);
}

void		get_reflection_ray(t_scene *scene)
{
	size_t	global;
	size_t	local;
	cl_mem	swap;
	cl_mem	swap2;
	cl_mem	swap_pointer;

	global = WID * HEI;
	local = 0;
	swap_pointer = scene->cl_data.scene.ray_buf;
	swap2 = scene->cl_data.scene.normal_buf;
	scene->cl_data.scene.ray_buf = scene->cl_data.scene.copy_normal_buf;
	scene->cl_data.scene.normal_buf = swap_pointer;
	scene->cl_data.scene.copy_normal_buf = swap2;
	swap = scene->cl_data.scene.intersection_buf;
	scene->cl_data.scene.intersection_buf = \
									scene->cl_data.scene.copy_intersec_buf;
	scene->cl_data.scene.copy_intersec_buf = swap;
	set_arg(scene, local);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
scene->cl_data.kernels[16], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}
