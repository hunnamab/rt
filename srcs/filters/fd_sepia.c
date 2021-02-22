/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_sepia.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 04:48:32 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 18:41:49 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd_sepia(t_filter_data *data)
{
	int		fd;
	int		ret;
	char	*str;
	int		err;

	err = 0;
	fd = open("./srcs/filters/sepia.cl", O_RDONLY);
	str = protected_malloc(sizeof(char), 64001);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[0] = clCreateProgramWithSource(\
	data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter sepia \
	program create error %d\n", err) : 0;
	clBuildProgram(data->programs[0], 1, \
	&data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter sepia \
	program build error %d\n", err) : 0;
	data->kernels[0] = clCreateKernel(\
	data->programs[0], "sepia", &err);
	err != 0 ? ft_printf("filter sepia \
	kernel compile error %d\n", err) : ft_printf("sepia kernel created\n");
	close(fd);
	free(str);
}
