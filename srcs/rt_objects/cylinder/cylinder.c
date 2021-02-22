/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:11:46 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/22 18:57:31 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			set_arg_cylinder(t_scene *scene, int index, cl_mem cs)
{
	clSetKernelArg(scene->cl_data.kernels[3], 0, sizeof(cl_mem),
										&scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 1, sizeof(cl_mem),
										&scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 2, sizeof(cl_mem),
										&scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 3, sizeof(t_cylinder),
										scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[3], 4, sizeof(cl_mem),
										&scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 5, sizeof(cl_int),
										(void*)&index);
	clSetKernelArg(scene->cl_data.kernels[3], 6, sizeof(cl_int),
										(void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[3], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[3], 8, sizeof(cl_int),
										(void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[3], 9, sizeof(cl_mem),
										&scene->cl_data.scene.material_buf);
}

void			intersect_ray_cylinder(t_scene *scene, int index,
													int is_refractive)
{
	size_t		global;
	size_t		local;
	cl_mem		cs;

	global = WID * HEI;
	is_refractive = 0;
	if (scene->objs[index]->cs_nmb > 0)
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY | \
	CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_cutting_surface) \
	* scene->objs[index]->cs_nmb, scene->objs[index]->cutting_surfaces, NULL);
	else
		cs = NULL;
	set_arg_cylinder(scene, index, cs);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[3],
					scene->cl_data.device_id,
					CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	ft_printf("local == max work group size == %ld\n", local);
	clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[3],
									1, NULL, &global, &local, 0, NULL, NULL);
}

void			one_argument_cylinder(char **description,
										t_scene *scene, int *snmi)
{
	t_object	*cylinder;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rad_spec[7];

	pos_vec_buf[0] = get_points(description[1]);
	rad_spec[0] = ftoi(get_coordinates(description[2]));
	pos_vec_buf[1] = get_points(description[3]);
	pos_vec_buf[2] = get_points(description[4]);
	color = get_color(description[5]);
	rad_spec[1] = ftoi(get_coordinates(description[6]));
	rad_spec[2] = ftoi(get_coordinates(description[7]));
	rad_spec[3] = ftoi(get_coordinates(description[8]));
	rad_spec[4] = ftoi(get_coordinates(description[9]));
	rad_spec[5] = ftoi(get_coordinates(description[10]));
	rad_spec[6] = ftoi(get_coordinates(description[11]));
	cylinder = new_cylinder(pos_vec_buf, rad_spec, color);
	cylinder->text = tex_new_bmp(get_file(description[12]));
	cylinder->normal_text = tex_new_bmp(get_file(description[13]));
	scene->objs[snmi[1]] = cylinder;
	snmi[1]++;
}

t_object		*multiple_cylinders(char **description, int i)
{
	t_object	*cylinder;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rad_spec[7];

	pos_vec_buf[0] = get_points(description[i + 1]);
	rad_spec[0] = ftoi(get_coordinates(description[i + 2]));
	pos_vec_buf[1] = get_points(description[i + 3]);
	pos_vec_buf[2] = get_points(description[i + 4]);
	color = get_color(description[i + 5]);
	rad_spec[1] = ftoi(get_coordinates(description[i + 6]));
	rad_spec[2] = ftoi(get_coordinates(description[i + 7]));
	rad_spec[3] = ftoi(get_coordinates(description[i + 8]));
	rad_spec[4] = ftoi(get_coordinates(description[i + 9]));
	rad_spec[5] = ftoi(get_coordinates(description[i + 10]));
	rad_spec[6] = ftoi(get_coordinates(description[i + 11]));
	cylinder = new_cylinder(pos_vec_buf, rad_spec, color);
	return (cylinder);
}
