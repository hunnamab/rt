/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_magic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:15:13 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/22 18:42:33 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd_magic(t_filter_data *data)
{
	int		fd;
	int		ret;
	char	*str;
	int		err;

	err = 0;
	str = protected_malloc(sizeof(char), 64001);
	fd = open("./srcs/filters/magic.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[MAGIC] = clCreateProgramWithSource(\
	data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter magic \
	program create error %d\n", err) : 0;
	clBuildProgram(data->programs[MAGIC], 1, \
	&data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter magic program build error %d\n", err) : 0;
	data->kernels[MAGIC] = clCreateKernel(\
	data->programs[MAGIC], "magic", &err);
	err != 0 ? ft_printf("filter magic kernel \
	compile error %d\n", err) : ft_printf("magic kernel created\n");
	close(fd);
	free(str);
}
