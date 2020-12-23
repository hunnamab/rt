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
	/* t_triangle *tri = (t_triangle *)scene->objs[0]->data;

	int i = 0;
	while (i < (WID * HEI))
	{
		intersect_ray_triangle_cl(scene->ray_buf, &scene->camera.position, scene->depth_buf, tri->vertex, i);
		i++;
	} */

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

	/* size_t global = WID * HEI;
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
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL); */
	
	/* size_t global = WID * HEI;

	cl_mem position;
	cl_mem vector;
	cl_mem angle;

	size_t local;
	t_cone *c = (t_cone *)scene->objs[0]->data;

	position = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	vector = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	angle = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, position, CL_FALSE, 0, sizeof(cl_float3), &c->position, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, vector, CL_FALSE, 0, sizeof(cl_float3), &c->vec, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, angle, CL_FALSE, 0, sizeof(float), &c->angle, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[2], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
    clSetKernelArg(scene->cl_data.kernels[2], 2, sizeof(cl_mem), &position);
	clSetKernelArg(scene->cl_data.kernels[2], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[2], 4, sizeof(cl_mem), &vector);
	clSetKernelArg(scene->cl_data.kernels[2], 5, sizeof(cl_mem), &angle);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[2], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[2], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL); */
	
	/* size_t global = WID * HEI;

	cl_mem position;
	cl_mem vector;
	cl_mem radius;

	size_t local;
	t_cylinder *cyl = (t_cylinder *)scene->objs[0]->data;

	position = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	vector = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	radius = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, position, CL_FALSE, 0, sizeof(cl_float3), &cyl->position, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, vector, CL_FALSE, 0, sizeof(cl_float3), &cyl->vec, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, radius, CL_FALSE, 0, sizeof(float), &cyl->radius, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[3], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
    clSetKernelArg(scene->cl_data.kernels[3], 2, sizeof(cl_mem), &position);
	clSetKernelArg(scene->cl_data.kernels[3], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 4, sizeof(cl_mem), &vector);
	clSetKernelArg(scene->cl_data.kernels[3], 5, sizeof(cl_mem), &radius);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[3], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[3], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL); */

	size_t global = WID * HEI;

	cl_mem vertex;

	size_t local;

	t_triangle *tri = (t_triangle *)scene->objs[0]->data;

	vertex = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  (sizeof(cl_float3) * 3), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, vertex, CL_FALSE, 0, (sizeof(cl_float3) * 3), &tri->vertex, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[4], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[4], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
	clSetKernelArg(scene->cl_data.kernels[4], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[4], 3, sizeof(cl_mem), &vertex);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[4], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[4], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL);

	/* size_t global = WID * HEI;

	cl_mem normal;
	cl_mem d;

	size_t local;
	t_plane *p = (t_plane *)scene->objs[0]->data;

	normal = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(cl_float3), NULL, NULL);
	d = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float), NULL, NULL);

	clEnqueueWriteBuffer(scene->cl_data.commands, normal, CL_FALSE, 0, sizeof(cl_float3), &p->normal, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, d, CL_FALSE, 0, sizeof(float), &p->d, 0, NULL, NULL);

	clSetKernelArg(scene->cl_data.kernels[5], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
	clSetKernelArg(scene->cl_data.kernels[5], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 3, sizeof(cl_mem), &normal);
	clSetKernelArg(scene->cl_data.kernels[5], 4, sizeof(cl_mem), &d);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[5], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[5], 1, NULL, &global, &local, 0, NULL, NULL);
    clFinish(scene->cl_data.commands);
    clEnqueueReadBuffer(scene->cl_data.commands, scene->cl_data.scene.depth_buf, CL_TRUE, 0, sizeof(float) * global, scene->depth_buf, 0, NULL, NULL); */

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
