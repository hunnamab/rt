/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers_part_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 10:14:23 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/19 10:17:19 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void				get_rays_arr(t_scene *scene)
{
	size_t			global;
	size_t			local;
	unsigned int	wid;
	unsigned int	hei;

	wid = WID;
	hei = HEI;
	global = WID * HEI;
	clSetKernelArg(scene->cl_data.kernels[0], 0, \
							sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[0], 1, \
							sizeof(cl_float3), &scene->camera.position);
	clSetKernelArg(scene->cl_data.kernels[0], 2, \
							sizeof(unsigned int), &wid);
	clSetKernelArg(scene->cl_data.kernels[0], 3, \
							sizeof(unsigned int), &hei);
	clSetKernelArg(scene->cl_data.kernels[0], 4, \
							sizeof(cl_float3), &scene->camera.rotation);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[0], \
			scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
						sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
scene->cl_data.kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}

static void			set_point_arg(t_scene *scene, int is_refractive)
{
	clSetKernelArg(scene->cl_data.kernels[18], 0, \
				sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[18], 1, \
				sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[18], 2, \
				sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[18], 3, \
				sizeof(cl_float3), &scene->camera.position);
	clSetKernelArg(scene->cl_data.kernels[18], 4, \
				sizeof(int), &scene->init_flag);
	clSetKernelArg(scene->cl_data.kernels[18], 5, \
				sizeof(cl_mem), &scene->cl_data.scene.exception_buf);
	clSetKernelArg(scene->cl_data.kernels[18], 6, \
				sizeof(int), &is_refractive);
}

void				get_closest_points(t_scene *scene, \
								float t, int is_refractive)
{
	size_t	global;
	size_t	local;
	int		i;

	global = WID * HEI;
	local = 256;
	t = 1;
	clFinish(scene->cl_data.commands);
	set_point_arg(scene, is_refractive);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
scene->cl_data.kernels[18], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
	i = -1;
	while (++i < scene->obj_nmb)
	{
		if (scene->objs[i]->refraction > 0.0)
			scene->has_refraction = 1;
		scene->objs[i]->intersect(scene, i, is_refractive);
		clFinish(scene->cl_data.commands);
	}
}
