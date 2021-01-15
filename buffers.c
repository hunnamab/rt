/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:29 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/15 14:32:06 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

void	get_rays_arr(t_scene *scene)
{
	size_t global;
	size_t local;

	global = WID * HEI;
	clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.camera, CL_FALSE, 0, sizeof(cl_float3), &scene->camera.position, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.viewport, CL_FALSE, 0, sizeof(cl_float3) * global, scene->viewport, 0, NULL, NULL);
    clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.ray_buf, CL_FALSE, 0, sizeof(cl_float3) * global, scene->ray_buf, 0, NULL, NULL);
    clSetKernelArg(scene->cl_data.kernels[0], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[0], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
    clSetKernelArg(scene->cl_data.kernels[0], 2, sizeof(cl_mem), &scene->cl_data.scene.viewport);
	clSetKernelArg(scene->cl_data.kernels[0], 3, sizeof(unsigned int), &global);
    clGetKernelWorkGroupInfo(scene->cl_data.kernels[0], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.ray_buf, CL_TRUE, 0, sizeof(cl_float3) * global, scene->ray_buf, 0, NULL, NULL);
}

void	get_closest_points(t_scene *scene, float t)
{
	size_t global = WID * HEI;
	int x = -1;
	int i = 0;
	while(++x < WID * HEI)
	{
		scene->index_buf[x] = -1;
		scene->depth_buf[x] = 100000000;
	}
	clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.index_buf, CL_FALSE, 0, sizeof(int) * global, scene->index_buf, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_FALSE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL);
	while (i < scene->obj_nmb)
	{
		scene->objs[i]->intersect(scene, i);
		clFinish(scene->cl_data.commands);
		i++;
	}
	clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL);
	clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.index_buf, CL_TRUE, 0, sizeof(int) * global, scene->index_buf, 0, NULL, NULL);
}

void	get_intersection_buf(t_scene *scene)
{
	size_t global = WID * HEI;
	size_t local;
	
	clEnqueueWriteBuffer(scene->cl_data.commands, scene->cl_data.scene.intersection_buf, CL_FALSE, 0, sizeof(cl_float3) * global, scene->intersection_buf, 0, NULL, NULL);
	
	clSetKernelArg(scene->cl_data.kernels[6], 0, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 1, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[6], 3, sizeof(cl_float3), (void *)&scene->camera.position);
	clSetKernelArg(scene->cl_data.kernels[6], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);

	clGetKernelWorkGroupInfo(scene->cl_data.kernels[6], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[6], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
	clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.intersection_buf, CL_TRUE, 0, sizeof(cl_float3) * global, scene->intersection_buf, 0, NULL, NULL);
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
	clSetKernelArg(scene->cl_data.kernels[7], 6, sizeof(cl_mem), &scene->cl_data.scene.camera);

	clGetKernelWorkGroupInfo(scene->cl_data.kernels[7], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    printf("sizeof t_primitive host %lu\n", sizeof(t_primitive));
	printf("sizeof cl_float3 host %lu\n", sizeof(cl_float3));
	printf("sizeof t_color host %lu\n", sizeof(t_color));
	printf("sizeof int host %lu\n", sizeof(cl_int));
	printf("sizeof float host %lu\n", sizeof(cl_float));
	printf("sizeof t_cylinder host %lu\n", sizeof(t_cylinder));
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[7], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
	clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.normal_buf, CL_TRUE, 0, sizeof(cl_float3) * global, scene->normal_buf, 0, NULL, NULL);
}
