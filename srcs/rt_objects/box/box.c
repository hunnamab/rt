/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:44:57 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 00:27:12 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			one_argument_box(char **description, t_scene *scene, int *snmi)
{
	cl_float3	cen_buf[2];
	float		specular[8];
	t_color		color;

	cen_buf[0] = get_points(description[1]);
	cen_buf[1] = get_points(description[2]);
	color = get_color(description[3]);
	specular[0] = ftoi(get_coordinates(description[4]));
	specular[1] = ftoi(get_coordinates(description[5]));
	specular[2] = ftoi(get_coordinates(description[6]));
	specular[3] = ftoi(get_coordinates(description[7]));
	specular[4] = ftoi(get_coordinates(description[8]));
	specular[5] = ftoi(get_coordinates(description[9]));
	specular[6] = ftoi(get_coordinates(description[10]));
	specular[7] = snmi[1];
	new_box(cen_buf, color, specular, scene);
	snmi[1] += 6;
}

void			multiple_boxes(char **description, int i, \
									t_scene *scene, int *snmi)
{
	cl_float3	cen_buf[2];
	float		specular[8];
	t_color		color;

	cen_buf[0] = get_points(description[i + 1]);
	cen_buf[1] = get_points(description[i + 2]);
	color = get_color(description[i + 3]);
	specular[0] = ftoi(get_coordinates(description[i + 4]));
	specular[1] = ftoi(get_coordinates(description[i + 5]));
	specular[2] = ftoi(get_coordinates(description[i + 6]));
	specular[3] = ftoi(get_coordinates(description[i + 7]));
	specular[4] = ftoi(get_coordinates(description[i + 8]));
	specular[5] = ftoi(get_coordinates(description[i + 9]));
	specular[6] = ftoi(get_coordinates(description[i + 10]));
	specular[7] = snmi[1];
	new_box(cen_buf, color, specular, scene);
}

static void		set_arg(t_scene *scene, int index, cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[11], 0, sizeof(cl_mem),
										&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 1, sizeof(cl_mem),
										&scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 2, sizeof(cl_mem),
										&scene->cl_data.scene.obj);
	clSetKernelArg(scene->cl_data.kernels[11], 3, sizeof(cl_mem),
										&scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 4, sizeof(cl_mem),
										&scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[11], 5, sizeof(cl_int),
													(void*)&index);
	clSetKernelArg(scene->cl_data.kernels[11], 6, sizeof(cl_int),
										(void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[11], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[11], 8, sizeof(cl_int),
										(void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[11], 9, sizeof(cl_mem),
										&scene->cl_data.scene.material_buf);
}

void			intersect_ray_box(t_scene *scene, int index, int is_refractive)
{
	size_t		global;
	size_t		local;
	cl_mem		cs;

	global = WID * HEI;
	is_refractive = 0;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
					sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb,
							scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	set_arg(scene, index, cs);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[11],
		scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE,
				sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands,
		scene->cl_data.kernels[11], 1, NULL, &global, &local, 0, NULL, NULL);
}
