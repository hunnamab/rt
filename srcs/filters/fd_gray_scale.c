/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_gray_scale.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:56:52 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 18:42:45 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd_gray_scale(t_filter_data *data)
{
	int		fd;
	int		ret;
	char	*str;
	int		err;

	err = 0;
	str = protected_malloc(sizeof(char), 64001);
	fd = open("./srcs/filters/grayscale.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[GRAYSCALE] = clCreateProgramWithSource(\
	data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter grayscale program \
	create error %d\n", err) : 0;
	clBuildProgram(data->programs[GRAYSCALE], 1, \
	&data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter grayscale program build error %d\n", err) : 0;
	data->kernels[GRAYSCALE] = clCreateKernel(\
	data->programs[GRAYSCALE], "grayscale", &err);
	err != 0 ? ft_printf("filter grayscale kernel \
	compile error %d\n", err) : ft_printf("grayscale kernel created\n");
	close(fd);
	free(str);
}
