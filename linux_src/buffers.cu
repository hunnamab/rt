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

void	check_funk(float3 *ray_arr, float3 *camera_start, float3 *viewport, int count)
{
	int x = 0;
	while(x < HEI * WID)
	{
		ray_arr[x].x = viewport[x].x - camera_start[0].x;
		ray_arr[x].y = viewport[x].y - camera_start[0].y;
		ray_arr[x].z = viewport[x].z - camera_start[0].z; 
		x++;
	}
}

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
/* 	int x = -1;
	int i = 0;
	while(++x < WID * HEI)
	{
		t = 0;
		i = -1;
		scene->index_buf[x] = - 1;
		scene->depth_buf[x] = 100000000;
		while (++i < scene->obj_nmb)
		{
			t = scene->objs[i]->intersect(scene, i, &scene->camera.position, &scene->ray_buf[x]);
			if (t < scene->depth_buf[x] && t != 0)
			{
				scene->depth_buf[x] = t;
				scene->index_buf[x] = i;
			}
		}
	} */
	int x = 0;
	scene->objs[0]->intersect(scene, 0, &scene->camera.position, &scene->ray_buf[x]);
	x = -1;
	while(x < (WID * HEI))
	{
		scene->index_buf[x] = 0;
		x++;
	}
	x = -1;
	while(++x < WID * HEI)
	{
		if (scene->depth_buf[x])
			scene->index_buf[x] = 0;
		else
		{
			scene->index_buf[x] = -1;
		}
	}
}

void	get_intersection_buf(t_scene *scene)
{
	int x;
	int y;
	int i;

	x = 0;
	y = 0;
	while (y < HEI)
	{
		while (x < WID)
		{
			i = y * WID + x;
			if (scene->index_buf[i] != -1)
			{
				scene->intersection_buf[i] = vector_scale(\
				&scene->ray_buf[i], scene->depth_buf[i]);
				scene->intersection_buf[i] = vector_add(\
				&scene->intersection_buf[i], &scene->camera.position);
			}
			else
				scene->intersection_buf[i] = get_point(0, 0, 0);
			x++;
		}
		x = 0;
		y++;
	}
}

void	get_normal_buf(t_scene *scene)
{
	int x;
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
	}
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
