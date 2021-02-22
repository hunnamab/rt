/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   device_objects_init.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 03:21:22 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 03:22:54 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	device_objects_init(t_scene *scene)
{
	t_object_d	*buf;
	int			i;

	buf = (t_object_d *)malloc(sizeof(t_object_d) * scene->obj_nmb);
	i = 0;
	while (i < scene->obj_nmb)
	{
		copy_memory_objects(scene, buf, i);
		init_buf(scene, buf, i);
		i++;
	}
	scene->cl_data.scene.obj = clCreateBuffer(scene->cl_data.context, \
															CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, \
							sizeof(t_object_d) * scene->obj_nmb, buf, NULL);
	free(buf);
}
