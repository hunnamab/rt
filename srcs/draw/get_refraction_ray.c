/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_refraction_ray.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 08:12:52 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 08:27:09 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		get_refract_continue(t_scene *scene)
{
	cl_mem	swap_pointer;

	swap_pointer = scene->cl_data.scene.ray_buf;
	scene->cl_data.scene.ray_buf = scene->cl_data.scene.normal_buf;
	scene->cl_data.scene.normal_buf = swap_pointer;
	get_closest_points(scene, 0, 1);
	get_intersection_buf(scene);
	get_normal_buf(scene);
	get_material_buf(scene, 1);
	get_frame_buf(scene, 1);
	clFinish(scene->cl_data.commands);
}

void		get_refraction_ray(t_scene *scene)
{
	size_t	global;
	size_t	local;

	global = WID * HEI;
	clSetKernelArg(scene->cl_data.kernels[15], 0, sizeof(cl_mem), \
								&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[15], 1, sizeof(cl_mem), \
								&scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[15], 2, sizeof(cl_mem), \
								&scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[15], 3, sizeof(cl_mem), \
								&scene->cl_data.scene.exception_buf);
	clSetKernelArg(scene->cl_data.kernels[15], 4, sizeof(cl_mem), \
								&scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[15], 5, sizeof(cl_mem), \
								&scene->cl_data.scene.material_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[15], \
								scene->cl_data.device_id, \
				CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
								scene->cl_data.kernels[15], 1, \
					NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
	get_refract_continue(scene);
}
