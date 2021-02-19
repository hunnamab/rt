/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_reflection_ray.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 08:33:38 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 08:33:51 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	get_reflection_ray(t_scene *scene)
{
	size_t global = WID * HEI;
	size_t local;
	cl_mem swap;
	cl_mem swap2;
	cl_mem swap_pointer = scene->cl_data.scene.ray_buf;
	swap2 = scene->cl_data.scene.normal_buf;
 	scene->cl_data.scene.ray_buf = scene->cl_data.scene.copy_normal_buf;
	scene->cl_data.scene.normal_buf = swap_pointer;
	scene->cl_data.scene.copy_normal_buf = swap2;
 	swap = scene->cl_data.scene.intersection_buf;
	scene->cl_data.scene.intersection_buf = scene->cl_data.scene.copy_intersec_buf;
	scene->cl_data.scene.copy_intersec_buf = swap; 
	clSetKernelArg(scene->cl_data.kernels[16], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[16], 1, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[16], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[16], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}