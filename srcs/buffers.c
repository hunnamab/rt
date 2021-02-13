/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:29 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/13 05:17:09 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

void	get_rays_arr(t_scene *scene)
{
	size_t global;
	size_t local;
	unsigned int wid = WID;
	unsigned int hei = HEI;
	global = WID * HEI;
 	clSetKernelArg(scene->cl_data.kernels[0], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[0], 1, sizeof(cl_float3), &scene->camera.position);
    clSetKernelArg(scene->cl_data.kernels[0], 2, sizeof(unsigned int), &wid);
	clSetKernelArg(scene->cl_data.kernels[0], 3, sizeof(unsigned int), &hei);
	clSetKernelArg(scene->cl_data.kernels[0], 4, sizeof(cl_float3), &scene->camera.rotation);
    clGetKernelWorkGroupInfo(scene->cl_data.kernels[0], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
}

void	get_closest_points(t_scene *scene, float t, int is_refractive)
{
	size_t global = WID * HEI;
	size_t local = 256;
	int x = -1;
	int i = 0;
	x = -1;
	clFinish(scene->cl_data.commands);
	/*это новый инит для индексбуфера, дипс буфера, эксепшн буфера, в случае, если это не проход для прозрачности*/
	clSetKernelArg(scene->cl_data.kernels[18], 0, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[18], 1, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
    clSetKernelArg(scene->cl_data.kernels[18], 2, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[18], 3, sizeof(cl_float3), &scene->camera.position);
	clSetKernelArg(scene->cl_data.kernels[18], 4, sizeof(int), &scene->init_flag);
	clSetKernelArg(scene->cl_data.kernels[18], 5, sizeof(cl_mem), &scene->cl_data.scene.exception_buf);
	clSetKernelArg(scene->cl_data.kernels[18], 6, sizeof(int), &is_refractive);
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[18], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
	while (i < scene->obj_nmb)
	{
 		if (scene->objs[i]->refraction > 0.0)
			scene->has_refraction = 1; 
		scene->objs[i]->intersect(scene, i, is_refractive);
		clFinish(scene->cl_data.commands);
		i++;
	}
}

void	get_intersection_buf(t_scene *scene)
{
	size_t global = WID * HEI;
	size_t local;
	
	clSetKernelArg(scene->cl_data.kernels[6], 0, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 1, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 3, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[6], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[6], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
}

void	get_normal_buf(t_scene *scene)
{
	size_t global = WID * HEI;
	size_t local;

	clSetKernelArg(scene->cl_data.kernels[7], 0, sizeof(cl_mem), &scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[7], 1, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 2, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 3, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 4, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 5, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[7], 6, sizeof(cl_float3), (void *)&scene->camera.position);
	clSetKernelArg(scene->cl_data.kernels[7], 7, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[7], 8, sizeof(cl_mem), &scene->cl_data.scene.textures);

	clGetKernelWorkGroupInfo(scene->cl_data.kernels[7], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[7], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
}

void get_frame_buf(t_scene *scene, int is_refractive)
{
	size_t global = WID * HEI;
	size_t local;
	
	clSetKernelArg(scene->cl_data.kernels[9], 0, sizeof(cl_mem), &scene->cl_data.scene.frame_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 1, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 2, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 3, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 5, sizeof(cl_mem), &scene->cl_data.scene.material_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 6, sizeof(cl_mem), &scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[9], 7, sizeof(cl_mem), &scene->cl_data.scene.light);
	clSetKernelArg(scene->cl_data.kernels[9], 8, sizeof(int), (void *)&scene->light_nmb);
	clSetKernelArg(scene->cl_data.kernels[9], 9, sizeof(int), (void *)&scene->obj_nmb);
	clSetKernelArg(scene->cl_data.kernels[9], 10, sizeof(int), (void *)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[9], 11, sizeof(cl_mem), &scene->cl_data.scene.prev_material_buf);
	clSetKernelArg(scene->cl_data.kernels[9], 12, sizeof(cl_int), (void *)&is_refractive);
	clSetKernelArg(scene->cl_data.kernels[9], 13, sizeof(cl_mem), &scene->cl_data.scene.frame_buf_refl);
	clSetKernelArg(scene->cl_data.kernels[9], 14, sizeof(cl_mem), &scene->cl_data.scene.frame_buf_refr);
	clSetKernelArg(scene->cl_data.kernels[9], 15, sizeof(cl_mem), &scene->cl_data.scene.orig_index_buf);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[9], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[9], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}
