/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers_material_buf.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:48:43 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/19 10:25:24 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

void	get_material_buf(t_scene *scene, int is_refractive)
{
	size_t gl_loc[2];

	gl_loc[0] = WID * HEI;
	clSetKernelArg(scene->cl_data.kernels[10], 0, sizeof(cl_mem), \
											&scene->cl_data.scene.textures);
	clSetKernelArg(scene->cl_data.kernels[10], 1, sizeof(cl_mem), \
											&scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[10], 2, sizeof(cl_mem), \
											&scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[10], 3, sizeof(cl_mem), \
									&scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[10], 4, sizeof(cl_mem), \
									&scene->cl_data.scene.material_buf);
	clSetKernelArg(scene->cl_data.kernels[10], 5, sizeof(cl_int), \
									(void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[10], 6, sizeof(cl_mem), \
									&scene->cl_data.scene.orig_index_buf);
	clSetKernelArg(scene->cl_data.kernels[10], 7, sizeof(cl_int), \
									(void *)&is_refractive);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[10], \
									scene->cl_data.device_id, \
			CL_KERNEL_WORK_GROUP_SIZE, sizeof(gl_loc[1]), &gl_loc[1], NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
	scene->cl_data.kernels[10], 1, NULL, &gl_loc[0], &gl_loc[1], 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}
