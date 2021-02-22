/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cl_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:48:55 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 04:24:26 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_device(t_scene *scene)
{
	cl_uint			np;
	cl_uint			status;
	cl_platform_id	pfs;
	char			*str;
	size_t			size;

	np = 0;
	status = clGetPlatformIDs(np, &pfs, NULL);
	clGetPlatformInfo(pfs, CL_PLATFORM_NAME, 0, NULL, &size);
	str = malloc(sizeof(char) * size);
	clGetPlatformInfo(pfs, CL_PLATFORM_NAME, size, str, NULL);
	free(str);
	clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, \
									&scene->cl_data.device_id, NULL);
	scene->cl_data.programs = malloc(sizeof(cl_program) * KERNEL_NUM);
	scene->cl_data.kernels = malloc(sizeof(cl_kernel) * KERNEL_NUM);
	scene->cl_data.context = clCreateContext(0, 1, &scene->cl_data.device_id, \
										NULL, NULL, NULL);
	scene->cl_data.commands = clCreateCommandQueue(scene->cl_data.context, \
								scene->cl_data.device_id, 0, NULL);
}

void	fd_init(t_scene *scene)
{
	fd2(scene);
	fd3(scene);
	fd4(scene);
	fd5(scene);
	fd6(scene);
	fd7(scene);
	fd8(scene);
	fd9(scene);
	fd10(scene);
	fd11(scene);
	fd12(scene);
	fd13(scene);
	fd14(scene);
	fd15(scene);
	fd16(scene);
	fd17(scene);
	fd18(scene);
	fd19(scene);
}

int		cl_init(t_scene *scene)
{
	char info[1024];

	init_device(scene);
	fd1(scene);
	clGetDeviceInfo(scene->cl_data.device_id, CL_DEVICE_NAME, 1024, info, NULL);
	ft_printf("%s\n", info);
	fd_init(scene);
	create_buf_gpu(scene);
	scene->filter_data.commands = scene->cl_data.commands;
	scene->filter_data.device_id = scene->cl_data.device_id;
	scene->filter_data.context = scene->cl_data.context;
	scene->filter_data.pixels = scene->cl_data.scene.frame_buf;
	load_textures(scene);
	device_objects_init(scene);
	clFinish(scene->cl_data.commands);
	return (0);
}
