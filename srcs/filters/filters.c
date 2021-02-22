/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:44:37 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 05:03:28 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filters.h"

/* у фильтров свои указатели на кернели с отдельной инициализацией, для использования
необходимо передать в t_filter_data.pixels указатель на буфер с пикселями в памяти видеокарты */
void	filters_init(t_filter_data *data)
{
	int fd;
	int ret;
	char *str;
	int err = 0;

	data->kernels = malloc(sizeof(cl_kernel) * 10);
	data->programs = malloc(sizeof(cl_program) * 10);
	fd = open("./srcs/filters/sepia.cl", O_RDONLY);
	str = protected_malloc(sizeof(char), 64001);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[0] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter sepia program create error %d\n", err) : 0;
	clBuildProgram(data->programs[0], 1, & data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter sepia program build error %d\n", err) : 0;
	 data->kernels[0] = clCreateKernel( data->programs[0], "sepia", &err);
	err != 0 ? ft_printf("filter sepia kernel compile error %d\n", err) : ft_printf("sepia kernel created\n");
	close(fd);
	ft_memset(str, 0, 64001);
	fd = open("./srcs/filters/negative.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[NEGATIVE] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter negative program create error %d\n", err) : 0;
	clBuildProgram(data->programs[NEGATIVE], 1, & data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter negative program build error %d\n", err) : 0;
	data->kernels[NEGATIVE] = clCreateKernel( data->programs[NEGATIVE], "negative", &err);
	err != 0 ? ft_printf("filter negative kernel compile error %d\n", err) : ft_printf("negative kernel created\n");
	ft_memset(str, 0, 64001);
	fd = open("./srcs/filters/grayscale.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[GRAYSCALE] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter grayscale program create error %d\n", err) : 0;
	clBuildProgram(data->programs[GRAYSCALE], 1, & data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter grayscale program build error %d\n", err) : 0;
	data->kernels[GRAYSCALE] = clCreateKernel( data->programs[GRAYSCALE], "grayscale", &err);
	err != 0 ? ft_printf("filter grayscale kernel compile error %d\n", err) : ft_printf("grayscale kernel created\n");
	ft_memset(str, 0, 64001);
	close(fd);
	fd = open("./srcs/filters/gauss.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[GAUSS] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter gauss program create error %d\n", err) : 0;
	clBuildProgram(data->programs[GAUSS], 1, & data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter gauss program build error %d\n", err) : 0;
	data->kernels[GAUSS] = clCreateKernel( data->programs[GAUSS], "gauss", &err);
	err != 0 ? ft_printf("filter gauss kernel compile error %d\n", err) : ft_printf("gauss kernel created\n");
	ft_memset(str, 0, 64001);
	close(fd);
	
	fd = open("./srcs/filters/sharpen.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[SHARPEN] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter sharpen program create error %d\n", err) : 0;
	clBuildProgram(data->programs[SHARPEN], 1, & data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter sharpen program build error %d\n", err) : 0;
	data->kernels[SHARPEN] = clCreateKernel( data->programs[SHARPEN], "sharpen", &err);
	err != 0 ? ft_printf("filter sharpen kernel compile error %d\n", err) : ft_printf("sharpen kernel created\n");
	ft_memset(str, 0, 64001);
	close(fd);

	fd = open("./srcs/filters/magic.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[MAGIC] = clCreateProgramWithSource( data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter magic program create error %d\n", err) : 0;
	clBuildProgram(data->programs[MAGIC], 1, & data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter magic program build error %d\n", err) : 0;
	data->kernels[MAGIC] = clCreateKernel( data->programs[MAGIC], "magic", &err);
	err != 0 ? ft_printf("filter magic kernel compile error %d\n", err) : ft_printf("magic kernel created\n");
	ft_memset(str, 0, 64001);
	close(fd);
	free(str);
}
