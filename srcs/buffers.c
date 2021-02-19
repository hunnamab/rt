/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:29 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/19 10:17:39 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		get_intersection_buf(t_scene *scene)
{
	size_t global;
	size_t local;

	global = WID * HEI;
	clSetKernelArg(scene->cl_data.kernels[6], 0, sizeof(cl_mem), \
								&scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 1, sizeof(cl_mem), \
								&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 2, sizeof(cl_mem), \
								&scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 3, sizeof(cl_mem), \
								&scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 4, sizeof(cl_mem), \
								&scene->cl_data.scene.index_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[6], \
				scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
					sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
scene->cl_data.kernels[6], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}

static void	set_normal_args(t_scene *scene)
{
	clSetKernelArg(scene->cl_data.kernels[7], 0, \
					sizeof(cl_mem), &scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[7], 1, \
					sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 2, \
					sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 3, \
					sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 4, \
					sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 5, \
					sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 6, \
					sizeof(cl_float3), (void *)&scene->camera.position);
	clSetKernelArg(scene->cl_data.kernels[7], 7, \
					sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[7], 8, \
		sizeof(cl_mem), &scene->cl_data.scene.textures);
}

void		get_normal_buf(t_scene *scene)
{
	size_t	global;
	size_t	local;

	global = WID * HEI;
	set_normal_args(scene);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[7], \
		scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
							sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
		scene->cl_data.kernels[7], 1, NULL, &global, \
										&local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}

static void	set_frame_args(t_scene *scene, int is_refractive)
{
	clSetKernelArg(scene->cl_data.kernels[9], 6, sizeof(cl_mem), \
										&scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[9], 7, sizeof(cl_mem), \
										&scene->cl_data.scene.light);
	clSetKernelArg(scene->cl_data.kernels[9], 8, sizeof(int), \
										(void *)&scene->light_nmb);
	clSetKernelArg(scene->cl_data.kernels[9], 9, sizeof(int), \
										(void *)&scene->obj_nmb);
	clSetKernelArg(scene->cl_data.kernels[9], 10, sizeof(int), \
										(void *)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[9], 11, sizeof(cl_mem), \
									&scene->cl_data.scene.prev_material_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 12, sizeof(cl_int), \
										(void *)&is_refractive);
	clSetKernelArg(scene->cl_data.kernels[9], 13, sizeof(cl_mem), \
										&scene->cl_data.scene.frame_buf_refl);
	clSetKernelArg(scene->cl_data.kernels[9], 14, sizeof(cl_mem), \
										&scene->cl_data.scene.frame_buf_refr);
	clSetKernelArg(scene->cl_data.kernels[9], 15, sizeof(cl_mem), \
										&scene->cl_data.scene.orig_index_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 16, sizeof(int), \
										(void *)&scene->has_refraction);
}

void		get_frame_buf(t_scene *scene, int is_refractive)
{
	size_t global;
	size_t local;

	global = WID * HEI;
	clSetKernelArg(scene->cl_data.kernels[9], 0, sizeof(cl_mem), \
										&scene->cl_data.scene.frame_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 1, sizeof(cl_mem), \
										&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 2, sizeof(cl_mem), \
										&scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 3, sizeof(cl_mem), \
										&scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 4, sizeof(cl_mem), \
										&scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 5, sizeof(cl_mem), \
										&scene->cl_data.scene.material_buf);
	set_frame_args(scene, is_refractive);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[9], \
										scene->cl_data.device_id, \
					CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[9], \
									1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}
