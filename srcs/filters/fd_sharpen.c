/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_sharpen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 18:15:13 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/22 18:41:11 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd_sharpen(t_filter_data *data)
{
	int		fd;
	int		ret;
	char	*str;
	int		err;

	err = 0;
	str = protected_malloc(sizeof(char), 64001);
	fd = open("./srcs/filters/sharpen.cl", O_RDONLY);
	ret = read(fd, str, 64000);
	str[ret] = '\0';
	data->programs[SHARPEN] = clCreateProgramWithSource(\
	data->context, 1, (const char **)&str, NULL, &err);
	err != 0 ? ft_printf("filter sharpen \
	program create error %d\n", err) : 0;
	clBuildProgram(data->programs[SHARPEN], \
	1, &data->device_id, NULL, NULL, &err);
	err != 0 ? ft_printf("filter sharpen \
	program build error %d\n", err) : 0;
	data->kernels[SHARPEN] = clCreateKernel(\
	data->programs[SHARPEN], "sharpen", &err);
	err != 0 ? ft_printf("filter sharpen kernel \
	compile error %d\n", err) : ft_printf("sharpen kernel created\n");
	close(fd);
	free(str);
}
