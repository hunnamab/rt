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

#include "rt.h"

void	check_funk(cl_float3 *ray_arr, cl_float3 *camera_start, cl_float3 *viewport, int count)
{
	int x = 0;
	int y = 0;
	int j = 0;
	int k = 0;
	while(x < HEI * WID)
	{
		ray_arr[x].x = viewport[x].x - camera_start[0].x;
		ray_arr[x].y = viewport[x].y - camera_start[0].y;
		ray_arr[x].z = viewport[x].z - camera_start[0].z; 
		x++;
	}
}

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
	/* int x = -1;
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
	}  */

	size_t global = WID * HEI;
	cl_mem s_center;
	cl_mem s_radius;
	size_t local;
	t_sphere *s = (t_sphere *)scene->objs[0]->data;
	s_center = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	s_radius = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, s_center, CL_FALSE, 0, sizeof(cl_float3), &s->center, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, s_radius, CL_FALSE, 0, sizeof(float), &s->radius, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[1], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
    clSetKernelArg(scene->cl_data.kernels[1], 2, sizeof(cl_mem), &s_center);
	clSetKernelArg(scene->cl_data.kernels[1], 3, sizeof(cl_mem), &s_radius);
	clSetKernelArg(scene->cl_data.kernels[1], 4, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[1], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[1], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL);
	int x = 0;
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
