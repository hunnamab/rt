/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paraboloid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:45:19 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 01:31:10 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		one_argument_paraboloid(char **description, \
						t_scene *scene, int *snmi, int surface_id)
{
	t_object	*paraboloid;
	cl_float3	cen_buf[3];
	float		rotation[3];
	float		specular[5];
	t_color		color;

	cen_buf[0] = get_points(description[1]);
	cen_buf[1] = get_points(description[2]);
	specular[0] = ftoi(get_coordinates(description[3]));
	cen_buf[2] = get_points(description[4]);
	rotation[0] = cen_buf[1].x;
	rotation[1] = cen_buf[1].y;
	rotation[2] = cen_buf[1].z;
	color = get_color(description[5]);
	specular[1] = ftoi(get_coordinates(description[6]));
	specular[2] = ftoi(get_coordinates(description[7]));
	specular[3] = ftoi(get_coordinates(description[8]));
	specular[4] = ftoi(get_coordinates(description[9]));
	surface_id = ftoi(get_coordinates(description[10]));
	paraboloid = new_paraboloid(cen_buf, color, specular, surface_id);
	paraboloid->text = tex_new_bmp(get_file(description[11]));
	scene->objs[snmi[1]] = paraboloid;
	snmi[1]++;
}

t_object	*multiple_paraboloids(char **description, int i)
{
	t_object	*paraboloid;
	cl_float3	cen_buf[3];
	float		specular[5];
	t_color		color;
	int			surface_id;

	cen_buf[0] = get_points(description[i + 1]);
	cen_buf[1] = get_points(description[i + 2]);
	specular[0] = ftoi(get_coordinates(description[i + 3]));
	cen_buf[2] = get_points(description[i + 4]);
	color = get_color(description[i + 5]);
	specular[1] = ftoi(get_coordinates(description[i + 6]));
	specular[2] = ftoi(get_coordinates(description[i + 7]));
	specular[3] = ftoi(get_coordinates(description[i + 8]));
	specular[4] = ftoi(get_coordinates(description[i + 9]));
	surface_id = ftoi(get_coordinates(description[i + 10]));
	paraboloid = new_paraboloid(cen_buf, color, specular, surface_id);
	return (paraboloid);
}

void		get_paraboloid(char **description, t_scene *scene, int *snmi)
{
	t_object	*paraboloid;
	int			i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				paraboloid = multiple_paraboloids(description, i);
				paraboloid->text = tex_new_bmp(get_file(description[i + 11]));
				scene->objs[snmi[1]] = paraboloid;
				snmi[1]++;
				i += 13;
			}
		}
	}
	else if (description[0][0] == '{')
		one_argument_paraboloid(description, scene, snmi, 0);
	else
		output_error(6);
}

static void	set_arg(t_scene *scene, int index, cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[12], 0, \
	sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[12], 1, \
	sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[12], 2, \
	sizeof(t_paraboloid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[12], 3, \
	sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[12], 4, \
	sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[12], 5, \
	sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[12], 6, \
	sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[12], 7, \
	sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[12], 8, \
	sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[12], 9, \
	sizeof(cl_mem), &scene->cl_data.scene.material_buf);
}

void		intersect_ray_paraboloid(t_scene *scene, \
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
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[12], \
	scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
	sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
	scene->cl_data.kernels[12], 1, NULL, &global, &local, 0, NULL, NULL);
	clFinish(scene->cl_data.commands);
}
