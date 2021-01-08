/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:38:29 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 17:28:32 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
extern "C"{
#include "rt_host.h"
}
#include "rt_device.cuh"

__host__ void	get_rays_arr(t_scene *scene)
{
	dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 1024;
    blockSize = 1024;
	if((cudaMemcpy(scene->device_data->viewport, scene->viewport, sizeof(float3) * WID * HEI, cudaMemcpyHostToDevice)) == CUDA_SUCCESS)
		printf("copy to device success\n");
	kernel_getray<<<gridSize, blockSize>>>(scene->device_data->viewport, scene->camera.position, scene->device_data->ray_buf);
	if((cudaMemcpy(scene->ray_buf, scene->device_data->ray_buf, sizeof(float3) * WID * HEI, cudaMemcpyDeviceToHost)) == CUDA_SUCCESS)
		printf("copy from device success\n");
}

void	get_closest_points(t_scene *scene, float t)
{
	int x = -1;
	int i = 0;
	while(++x < WID * HEI)
	{
		scene->index_buf[x] = -1;
		scene->depth_buf[x] = 100000000;
	}
	cudaMemcpy(scene->device_data->index_buf, scene->index_buf, sizeof(int) * WID * HEI, cudaMemcpyHostToDevice);
	cudaMemcpy(scene->device_data->depth_buf, scene->depth_buf, sizeof(float) * WID * HEI, cudaMemcpyHostToDevice);
	i = 0;
	while(i < scene->obj_nmb)
	{
		scene->objs[i]->intersect(scene, i);
		i++;
	}
	if(scene->mode == 2)
	{
		cudaMemcpy(scene->index_buf, scene->device_data->index_buf, sizeof(int) * WID * HEI, cudaMemcpyDeviceToHost);
		cudaMemcpy(scene->depth_buf, scene->device_data->depth_buf, sizeof(float) * WID * HEI, cudaMemcpyDeviceToHost);
	}
}

void	get_intersection_buf(t_scene *scene)
{
	dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	kernel_get_intersection_point<<<gridSize, blockSize>>>(scene->device_data->intersection_buf,scene->device_data->ray_buf,scene->device_data->depth_buf, scene->camera.position, scene->device_data->index_buf);
}

void	get_normal_buf(t_scene *scene)
{
	dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	get_normal_buf_device<<<gridSize, blockSize>>>(scene->device_data->obj, scene->device_data->ray_buf, scene->device_data->index_buf, scene->device_data->normal_buf, scene->device_data->intersection_buf, scene->device_data->depth_buf, scene->camera.position); 
}

void	get_material_buf(t_scene *scene)
{
	dim3     gridSize;
    dim3     blockSize;
	int err;
	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	get_material_buf_device<<<gridSize, blockSize>>>(scene->device_data->index_buf, scene->device_data->material_buf, scene->device_data->obj);
	if(scene->mode == 3)
	{
		cudaMemcpy(scene->index_buf, scene->device_data->index_buf, sizeof(int) * WID * HEI, cudaMemcpyDeviceToHost);
		cudaMemcpy(scene->material_buf, scene->device_data->material_buf, sizeof(t_material) * WID * HEI, cudaMemcpyDeviceToHost);
	}
}

 void	get_frame_buf(t_scene *scene)
{
	dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	get_frame_buf_device<<<gridSize, blockSize>>>(scene->device_data->frame_buf, scene->device_data->ray_buf, scene->device_data->intersection_buf,scene->device_data->index_buf, scene->device_data->normal_buf, scene->device_data->material_buf, scene->device_data->obj, scene->device_data->light, scene->light_nmb);
	cudaMemcpy(scene->frame_buf, scene->device_data->frame_buf, sizeof(t_color) * WID * HEI, cudaMemcpyDeviceToHost);
}
