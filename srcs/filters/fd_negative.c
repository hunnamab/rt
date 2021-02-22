/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_negative.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:53:49 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:56:34 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd_negative(t_filter_data *data)
{
	int		fd;
	char	*str;
	int		ret;
	int		err;

	ft_memset(str, 0, 64001);
	fd = open("./srcs/filters/negative.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[NEGATIVE] = clCreateProgramWithSource(data->context, \
										1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter negative program create error %d\n", err) : 0;
	clBuildProgram(data->programs[NEGATIVE], 1, \
										&data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter negative program build error %d\n", err) : 0;
	data->kernels[NEGATIVE] = clCreateKernel(data->programs[NEGATIVE], \
														"negative", &err);
	err != 0 ? ft_printf("filter negative kernel compile error %d\n", err) \
									: ft_printf("negative kernel created\n");
	close(fd);
	free(str);
}
