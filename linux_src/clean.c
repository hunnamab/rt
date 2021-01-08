/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hunnamab <hunnamab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/08 17:32:12 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 14:58:31 by hunnamab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_host.h"

void	clear_default(t_object *obj)
{
	free(obj->data);
	free(obj);
}

void	clean_scene(t_scene *scene)
{
	int i;

	i = 0;
	ft_memdel((void **)&scene->normal_buf);
	ft_memdel((void **)&scene->material_buf);
	ft_memdel((void **)&scene->intersection_buf);
	ft_memdel((void **)&scene->ray_buf);
	ft_memdel((void **)&scene->viewport);
	ft_memdel((void **)&scene->index_buf);
	ft_memdel((void **)&scene->depth_buf);
/* 	clReleaseMemObject(scene->cl_data.scene.ray_buf);
    clReleaseMemObject(scene->cl_data.scene.intersection_buf);
	clReleaseMemObject(scene->cl_data.scene.viewport);
	clReleaseMemObject(scene->cl_data.scene.index_buf);
	clReleaseMemObject(scene->cl_data.scene.depth_buf);
	clReleaseMemObject(scene->cl_data.scene.normal_buf);
	clReleaseMemObject(scene->cl_data.scene.camera);
    clReleaseProgram(scene->cl_data.programs[0]);
    clReleaseKernel(scene->cl_data.kernels[0]);
    clReleaseCommandQueue(scene->cl_data.commands);
    clReleaseContext(scene->cl_data.context); */
	while (i < scene->obj_nmb)
	{
		scene->objs[i]->clear_obj(scene->objs[i]);
		i++;
	}
	i = 0;
	while (i < scene->light_nmb)
	{
		ft_memdel((void **)&scene->light[i]);
		i++;
	}
	free(scene->light);
	free(scene->objs);
	ft_memdel((void **)&scene);
}
