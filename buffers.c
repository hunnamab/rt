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

const char *get_ray_arr = "\n" \
"__kernel void square(                                                  \n" \
"   __global float* ray_arr,                                            \n" \
"   __global float* camera_start,                                       \n" \
"    __global float *viewport,                                          \n" \
"   const unsigned int count)                                           \n" \
"{                                                                      \n" \
"   int i = get_global_id(0);                                           \n" \
"   int k = i * 3;                                 						\n" \
"   ray_arr[k] = viewport[k] - camera_start[0];                     	\n" \
"   ray_arr[k + 1] = viewport[k + 1] - camera_start[1];             	\n" \
"   ray_arr[k + 2] = viewport[k + 2] - camera_start[2];             	\n" \
"}                                                                      \n" \
"\n";

void	check_funk(float *ray_arr, float *camera_start, float *viewport, int count)
{
	int x = 0;
	int y = 0;
	int j = 0;
	int k = 0;
	while(x < HEI * WID)
	{
		k = x * 3;
		ray_arr[k] = viewport[k] - camera_start[0];
		ray_arr[k + 1] = viewport[k + 1] - camera_start[1];
		ray_arr[k + 2] = viewport[k + 2] - camera_start[2]; 
		x++;
	}
}

void	get_rays_arr(t_scene *scene)
{
	int x;
	int y;
	int j = 0;
	float *ray_arr;
	float *camera_start;
	float *viewport;
	int k;
	camera_start = malloc(sizeof(float) * 3);
	camera_start[0] = scene->camera.position.x;
	camera_start[1] = scene->camera.position.y;
	camera_start[2] = scene->camera.position.z;
	ray_arr = malloc(sizeof(float) * WID * HEI * 3);
	viewport = malloc(sizeof(float) * 3 * WID * HEI);
	x = 0;
	y = 0;
/*  	while (y < HEI)
	{
		while (x < WID)
		{
			j = y * WID + x;
			scene->ray_buf[j].start.x = scene->camera.position.x;
			scene->ray_buf[j].start.y = scene->camera.position.y;
			scene->ray_buf[j].start.z = scene->camera.position.z;
			scene->ray_buf[j].dir = vector_sub(&scene->viewport[j], \
												&scene->camera.position);			
			x++;
		}
		x = 0;
		y++;
	} */
	while (y < HEI)
	{
		while (x < WID)
		{
			j = y * WID + x;
			k = y * (WID * 3) + (x * 3);
			viewport[k] = scene->viewport[j].x;
			viewport[k + 1] = scene->viewport[j].y;
			viewport[k + 2] = scene->viewport[j].z;
			scene->ray_buf[j].start.x = camera_start[0];
			scene->ray_buf[j].start.y = camera_start[1];
			scene->ray_buf[j].start.z = camera_start[2];
			x++;
		}
		x = 0;
		y++;
	}
	//check_funk(ray_arr, camera_start, viewport, (WID * HEI));
	//STARTING STUFF
	int err;
    unsigned int correct;             // number of correct results returned
    unsigned int count = WID * HEI * 3;
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
	cl_mem kernel_viewport;
	cl_mem kernel_camera_start;
	cl_mem kernel_ray_arr;
    int i;
    err = 0;
    err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &scene->cl_data.device_id, NULL);
    scene->cl_data.programs = malloc(sizeof(cl_program) * 1);
    scene->cl_data.kernels = malloc(sizeof(cl_kernel) * 1);
    scene->cl_data.context = clCreateContext(0, 1, &scene->cl_data.device_id, NULL, NULL, &err);
    scene->cl_data.commands = clCreateCommandQueue(scene->cl_data.context, scene->cl_data.device_id, 0, &err);
    scene->cl_data.programs[0] = clCreateProgramWithSource(scene->cl_data.context, 1, (const char **)&get_ray_arr, NULL, &err);
    err = clBuildProgram(scene->cl_data.programs[0], 0, NULL, NULL, NULL, NULL);
    scene->cl_data.kernels[0] = clCreateKernel(scene->cl_data.programs[0], "square", &err);
    kernel_ray_arr = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float) * count, NULL, NULL);
	kernel_viewport = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float) * count, NULL, NULL);
	kernel_camera_start = clCreateBuffer(scene->cl_data.context,  CL_MEM_READ_WRITE,  sizeof(float) * 3, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, kernel_camera_start, CL_FALSE, 0, sizeof(float) * 3, camera_start, 0, NULL, NULL);
	clEnqueueWriteBuffer(scene->cl_data.commands, kernel_viewport, CL_FALSE, 0, sizeof(float) * count, viewport, 0, NULL, NULL);
    clEnqueueWriteBuffer(scene->cl_data.commands, kernel_ray_arr, CL_FALSE, 0, sizeof(float) * count, ray_arr, 0, NULL, NULL);
    err = 0;
	count = WID * HEI;
	global = WID * HEI;
    err  = clSetKernelArg(scene->cl_data.kernels[0], 0, sizeof(cl_mem), &kernel_ray_arr);
    err |= clSetKernelArg(scene->cl_data.kernels[0], 1, sizeof(cl_mem), &kernel_camera_start);
    err |= clSetKernelArg(scene->cl_data.kernels[0], 2, sizeof(cl_mem), &kernel_viewport);
	err |= clSetKernelArg(scene->cl_data.kernels[0], 3, sizeof(unsigned int), &count);

    // Get the maximum work group size for executing the kernel on the device
    //
    err = clGetKernelWorkGroupInfo(scene->cl_data.kernels[0], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    err = clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[0], 1, NULL, &global, &local, 0, NULL, NULL);
    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(scene->cl_data.commands);
    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer(scene->cl_data.commands, kernel_ray_arr, CL_TRUE, 0, sizeof(float) * count * 3, ray_arr, 0, NULL, NULL);   
    // Shutdown and cleanup
    //
    clReleaseMemObject(kernel_ray_arr);
    clReleaseProgram(scene->cl_data.programs[0]);
    clReleaseKernel(scene->cl_data.kernels[0]);
    clReleaseCommandQueue(scene->cl_data.commands);
    clReleaseContext(scene->cl_data.context);
	//END STUFF
 	x = 0;
	y = 0;
	while (y < HEI)
	{
		while (x < WID)
		{
			j = y * WID + x;
			k = y * (WID * 3) + (x * 3);
			scene->ray_buf[j].dir.x = ray_arr[k];
			scene->ray_buf[j].dir.y = ray_arr[k + 1];
			scene->ray_buf[j].dir.z = ray_arr[k + 2];
			x++;
		}
		x = 0;
		y++;
	}
}

void	get_closest_points(t_scene *scene, double t)
{
	int		ixyj[4];

	ixyj[2] = -1;
	while (++ixyj[2] < HEI)
	{
		ixyj[1] = -1;
		while (++ixyj[1] < WID)
		{
			t = 0;
			ixyj[0] = -1;
			ixyj[3] = ixyj[2] * WID + ixyj[1];
			scene->index_buf[ixyj[3]] = -1;
			scene->depth_buf[ixyj[3]] = 100000000;
			while (++ixyj[0] < scene->obj_nmb)
			{
				t = scene->objs[ixyj[0]]->intersect(&scene->ray_buf[ixyj[3]], \
												scene->objs[ixyj[0]]);
				if (t < scene->depth_buf[ixyj[3]] && t != 0)
				{
					scene->depth_buf[ixyj[3]] = t;
					scene->index_buf[ixyj[3]] = ixyj[0];
				}
			}
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
				&scene->ray_buf[i].dir, scene->depth_buf[i]);
				scene->intersection_buf[i] = vector_add(\
				&scene->intersection_buf[i], &scene->ray_buf[i].start);
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
