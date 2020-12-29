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
#include "rt_cuda.h"
}
#include "rt.cuh"

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
		scene->objs[i]->intersect(scene, i, &scene->camera.position, &scene->ray_buf[x]);
		i++;
	}
	cudaMemcpy(scene->index_buf, scene->device_data->index_buf, sizeof(int) * WID * HEI, cudaMemcpyDeviceToHost);
	cudaMemcpy(scene->depth_buf, scene->device_data->depth_buf, sizeof(float) * WID * HEI, cudaMemcpyDeviceToHost);
}

void	get_intersection_buf(t_scene *scene)
{
	dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	kernel_get_intersection_point<<<gridSize, blockSize>>>(scene->device_data->intersection_buf,scene->device_data->ray_buf,scene->device_data->depth_buf, scene->camera.position, scene->device_data->index_buf);
	cudaMemcpy(scene->intersection_buf, scene->device_data->intersection_buf, sizeof(float3) * WID * HEI, cudaMemcpyDeviceToHost);
}

void	get_normal_buf(t_scene *scene)
{
/* 	int x;
	int y;
	int i;
	int j;

	j = 0;
	x = 0;
	y = 0;
	while (y < HEI)
	{
		while (x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				j = scene->index_buf[i];
				scene->objs[j]->get_normal(scene, i, j);
			}
			else
				scene->normal_buf[j] = get_point(0, 0, 0);
			x++;
		}
		x = 0;
		y++;
	} */
	cudaMalloc(&scene->device_data->obj, sizeof(t_object_d) * scene->obj_nmb);
	t_object_d *buf;
	buf = (t_object_d *)malloc(sizeof(t_object_d) * scene->obj_nmb);
	int i = 0;
	while(i < scene->obj_nmb)
	{
		/* if(scene->objs[i]->type == SPHERE) */
		t_sphere *s;
		s = reinterpret_cast<t_sphere *>(scene->objs[i]->data);
		buf->sphere.center = s->center;
		buf->sphere.radius = s->radius;
		i++;
	}
	cudaMemcpy(scene->device_data->obj, buf, sizeof(t_object_d) * scene->obj_nmb, cudaMemcpyHostToDevice);
	dim3     gridSize;
    dim3     blockSize;

	gridSize = WID * HEI / 1024;
	blockSize = 1024;
	get_normal_buf_device<<<gridSize, blockSize>>>(scene->device_data->obj, scene->device_data->ray_buf, scene->device_data->index_buf, scene->device_data->normal_buf, scene->device_data->intersection_buf); 
	cudaMemcpy(scene->normal_buf, scene->device_data->normal_buf, sizeof(float3) * WID * HEI, cudaMemcpyDeviceToHost);
}

void	get_material_buf(t_scene *scene)
{
	int x;
	int y;
	int i;

	y = -1;
	while (++y < HEI)
	{
		x = -1;
		while (++x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				copy_color(&scene->material_buf[i].color, \
							&scene->objs[scene->index_buf[i]]->color);
				scene->material_buf[i].specular = \
				scene->objs[scene->index_buf[i]]->specular;
			}
			else
			{
				set_color_zero(&scene->material_buf[i].color);
				scene->material_buf[i].specular = -1;
			}
		}
	}
}
