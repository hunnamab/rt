/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_gauss.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:15:13 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/22 18:42:57 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd_gauss(t_filter_data *data)
{
	int		fd;
	int		ret;
	char	*str;
	int		err;

	err = 0;
	fd = open("./srcs/filters/gauss.cl", O_RDONLY);
	str = protected_malloc(sizeof(char), 64001);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[GAUSS] = clCreateProgramWithSource(\
	data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter gauss program \
	create error %d\n", err) : 0;
	clBuildProgram(data->programs[GAUSS], 1, \
	&data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter gauss program build error %d\n", err) : 0;
	data->kernels[GAUSS] = clCreateKernel(\
	data->programs[GAUSS], "gauss", &err);
	err != 0 ? ft_printf("filter gauss kernel \
	compile error %d\n", err) : ft_printf("gauss kernel created\n");
	close(fd);
	free(str);
}
