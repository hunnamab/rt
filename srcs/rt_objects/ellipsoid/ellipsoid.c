/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:45:06 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 01:00:44 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		one_argument_ellipsoid(char **description, \
							t_scene *scene, int *snmi)
{
	t_object	*ellipsoid;
	cl_float3	cen_buf[2];
	float		specular[7];
	t_color		color;
	int			surface_id;

	cen_buf[0] = get_points(description[1]);
	specular[0] = ftoi(get_coordinates(description[2]));
	specular[1] = ftoi(get_coordinates(description[3]));
	specular[2] = ftoi(get_coordinates(description[4]));
	cen_buf[1] = get_points(description[5]);
	color = get_color(description[6]);
	specular[3] = ftoi(get_coordinates(description[7]));
	specular[4] = ftoi(get_coordinates(description[8]));
	specular[5] = ftoi(get_coordinates(description[9]));
	specular[6] = ftoi(get_coordinates(description[10]));
	surface_id = ftoi(get_coordinates(description[11]));
	ellipsoid = new_ellipsoid(cen_buf, color, specular, surface_id);
	ellipsoid->text = tex_new_bmp(get_file(description[12]));
	scene->objs[snmi[1]] = ellipsoid;
	snmi[1]++;
}

t_object	*multiple_ellipsoids(char **description, int i)
{
	t_object	*ellipsoid;
	cl_float3	cen_buf[2];
	float		specular[7];
	t_color		color;
	int			surface_id;

	cen_buf[0] = get_points(description[i + 1]);
	specular[0] = ftoi(get_coordinates(description[i + 2]));
	specular[1] = ftoi(get_coordinates(description[i + 3]));
	specular[2] = ftoi(get_coordinates(description[i + 4]));
	cen_buf[1] = get_points(description[i + 5]);
	color = get_color(description[i + 6]);
	specular[3] = ftoi(get_coordinates(description[i + 7]));
	specular[4] = ftoi(get_coordinates(description[i + 8]));
	specular[5] = ftoi(get_coordinates(description[i + 9]));
	specular[6] = ftoi(get_coordinates(description[i + 10]));
	surface_id = ftoi(get_coordinates(description[i + 11]));
	ellipsoid = new_ellipsoid(cen_buf, color, specular, surface_id);
	return (ellipsoid);
}

void		get_ellipsoid(char **description, t_scene *scene, int *snmi)
{
	t_object	*ellipsoid;
	int			i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				ellipsoid = multiple_ellipsoids(description, i);
				ellipsoid->text = tex_new_bmp(get_file(description[i + 12]));
				scene->objs[snmi[1]] = ellipsoid;
				snmi[1]++;
				i += 14;
			}
		}
	}
	else if (description[0][0] == '{')
		one_argument_ellipsoid(description, scene, snmi);
	else
		output_error(6);
}

static void	set_arg(t_scene *scene, int index, cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[8], 0, \
	sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 1, \
	sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 2, \
	sizeof(t_ellipsoid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[8], 3, \
	sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 4, \
	sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 5, \
	sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[8], 6, \
	sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[8], 7, \
	sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[8], 8, \
	sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[8], 9, \
	sizeof(cl_mem), &scene->cl_data.scene.material_buf);
}

void		intersect_ray_ellipsoid(t_scene *scene, \
							int index, int is_refractive)
{
	size_t	global;
	size_t	local;
	cl_mem	cs;

	global = WID * HEI;
	is_refractive = 0;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, \
		sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb, \
		scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	set_arg(scene, index, cs);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[8], \
	scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
			sizeof(local), &local, NULL);
	ft_printf("ellipsoid local == %ld\n", local);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
	scene->cl_data.kernels[8], 1, NULL, &global, &local, 0, NULL, NULL);
}
