/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/08 17:32:12 by pmetron           #+#    #+#             */
/*   Updated: 2021/01/16 18:48:01 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
	clReleaseMemObject(scene->cl_data.scene.ray_buf);
    clReleaseMemObject(scene->cl_data.scene.intersection_buf);
	clReleaseMemObject(scene->cl_data.scene.viewport);
	clReleaseMemObject(scene->cl_data.scene.index_buf);
	clReleaseMemObject(scene->cl_data.scene.depth_buf);
	clReleaseMemObject(scene->cl_data.scene.normal_buf);
	clReleaseMemObject(scene->cl_data.scene.camera);
	clReleaseMemObject(scene->cl_data.scene.obj);
	while (i < 9)
	{
		clReleaseProgram(scene->cl_data.programs[i]);
		clReleaseKernel(scene->cl_data.kernels[i]);
		i++;	
	}
    clReleaseCommandQueue(scene->cl_data.commands);
    clReleaseContext(scene->cl_data.context);
	ft_memdel((void **)&scene->cl_data.kernels);
	ft_memdel((void **)&scene->cl_data.programs);
	while (i < scene->obj_nmb)
	{
		scene->objs[i]->clear_obj(scene->objs[i]);
		i++;
	}
	i = 0;
/* 	while (i < scene->light_nmb)
	{
		ft_memdel((void **)&scene->light[i]);
		i++;
	} */
	free(scene->light);
	free(scene->objs);
	ft_memdel((void **)&scene);
}
