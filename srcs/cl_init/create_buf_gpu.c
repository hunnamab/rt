/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_buf_gpu.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:12:07 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:19:41 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	create_buf_gpu_part2(t_scene *scene)
{
	int				err;
	unsigned int	count;

	count = WID * HEI;
	scene->cl_data.scene.normal_buf = clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_WRITE, sizeof(cl_float3) * count, NULL, &err);
	scene->cl_data.scene.copy_normal_buf = \
				clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_WRITE, sizeof(cl_float3) * count, NULL, &err);
	scene->cl_data.scene.frame_buf = clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_WRITE, sizeof(t_color) * count, NULL, &err);
	scene->cl_data.scene.frame_buf_refl = \
				clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_WRITE, sizeof(t_color) * count, NULL, &err);
	scene->cl_data.scene.frame_buf_refr = \
				clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_WRITE, sizeof(t_color) * count, NULL, &err);
	scene->cl_data.scene.light = clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, \
	sizeof(t_light) * scene->light_nmb, scene->light, &err);
	scene->cl_data.scene.material_buf = clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_WRITE, sizeof(t_material) * count, NULL, &err);
	scene->cl_data.scene.prev_material_buf = \
	clCreateBuffer(scene->cl_data.context, CL_MEM_READ_WRITE, \
	sizeof(t_material) * count, NULL, &err);
}

void	create_buf_gpu(t_scene *scene)
{
	int				err;
	unsigned int	count;

	err = 0;
	count = WID * HEI;
	scene->cl_data.scene.ray_buf = clCreateBuffer(scene->cl_data.context, \
	CL_MEM_READ_WRITE, sizeof(cl_float3) * count, NULL, &err);
	scene->cl_data.scene.intersection_buf = \
				clCreateBuffer(scene->cl_data.context, \
		CL_MEM_READ_WRITE, sizeof(cl_float3) * count, NULL, &err);
	scene->cl_data.scene.copy_intersec_buf = \
				clCreateBuffer(scene->cl_data.context, \
			CL_MEM_READ_WRITE, sizeof(cl_float3) * count, NULL, &err);
	scene->cl_data.scene.index_buf = \
				clCreateBuffer(scene->cl_data.context, 0, \
						sizeof(int) * count, NULL, &err);
	scene->cl_data.scene.orig_index_buf = \
				clCreateBuffer(scene->cl_data.context, 0, \
						sizeof(int) * count, NULL, &err);
	scene->cl_data.scene.exception_buf = \
				clCreateBuffer(scene->cl_data.context, 0, \
						sizeof(int) * count, NULL, &err);
	scene->cl_data.scene.depth_buf = clCreateBuffer(scene->cl_data.context, 0, \
	sizeof(float) * count, NULL, &err);
	create_buf_gpu_part2(scene);
}
