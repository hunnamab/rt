/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   torus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:45:29 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 02:36:09 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		one_argument_torus(char **description, t_scene *scene, int *snmi)
{
	t_object	*torus;
	cl_float3	cen_buf[3];
	float		specular[6];
	t_color		color;
	int			surface_id;

	cen_buf[0] = get_points(description[1]);
	cen_buf[1] = get_points(description[2]);
	specular[0] = ftoi(get_coordinates(description[3]));
	specular[1] = ftoi(get_coordinates(description[4]));
	cen_buf[2] = get_points(description[5]);
	color = get_color(description[6]);
	specular[2] = ftoi(get_coordinates(description[7]));
	specular[3] = ftoi(get_coordinates(description[8]));
	specular[4] = ftoi(get_coordinates(description[9]));
	specular[5] = ftoi(get_coordinates(description[10]));
	surface_id = ftoi(get_coordinates(description[11]));
	torus = new_torus(cen_buf, color, specular, surface_id);
	torus->text = tex_new_bmp(get_file(description[12]));
	scene->objs[snmi[1]] = torus;
	snmi[1]++;
}

t_object	*multiple_torus(char **description, int i)
{
	t_object	*torus;
	cl_float3	cen_buf[3];
	float		specular[6];
	t_color		color;
	int			surface_id;

	cen_buf[0] = get_points(description[i + 1]);
	cen_buf[1] = get_points(description[i + 2]);
	specular[0] = ftoi(get_coordinates(description[i + 3]));
	specular[1] = ftoi(get_coordinates(description[i + 4]));
	cen_buf[2] = get_points(description[i + 5]);
	color = get_color(description[i + 6]);
	specular[2] = ftoi(get_coordinates(description[i + 7]));
	specular[3] = ftoi(get_coordinates(description[i + 8]));
	specular[4] = ftoi(get_coordinates(description[i + 9]));
	specular[5] = ftoi(get_coordinates(description[i + 10]));
	surface_id = ftoi(get_coordinates(description[i + 11]));
	torus = new_torus(cen_buf, color, specular, surface_id);
	return (torus);
}

void		get_torus(char **description, t_scene *scene, int *snmi)
{
	t_object	*torus;
	int			i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				torus = multiple_torus(description, i);
				torus->text = tex_new_bmp(get_file(description[i + 12]));
				scene->objs[snmi[1]] = torus;
				snmi[1]++;
				i += 14;
			}
		}
	}
	else if (description[0][0] == '{')
		one_argument_torus(description, scene, snmi);
	else
		output_error(6);
}

void		intersect_ray_torus(t_scene *scene, int index, int is_refractive)
{
	size_t global;
	size_t local;
	cl_mem cs;

	global = WID * HEI;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, \
		sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb, \
		scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	set_arg_torus(scene, index, cs);
	set_arg2_torus(scene, index, is_refractive);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[13], \
	scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
	sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
	scene->cl_data.kernels[13], 1, NULL, &global, &local, 0, NULL, NULL);
}
