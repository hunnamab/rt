/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 22:45:20 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/22 02:20:45 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

void			get_sphere_normal(t_scene *scene, int index, int obj_num)
{
	t_sphere *s;

	s = (t_sphere *)scene->objs[obj_num]->data;
	scene->normal_buf[index] = \
	vector_sub(&scene->intersection_buf[index], &s->center);
	scene->normal_buf[index] = vector_div_by_scalar(&scene->normal_buf[index], \
	vector_length(&scene->normal_buf[index]));
	if (vector_dot(&scene->ray_buf[index], \
	&scene->normal_buf[index]) > 0.0001)
		scene->normal_buf[index] = vector_scale(&scene->normal_buf[index], -1);
}

void			intersect_ray_sphere(t_scene *scene, \
							int index, int is_refractive)
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
	set_arg_sphere(scene, index, cs);
	set_arg2_sphere(scene, index, is_refractive);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[1], \
			scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
				sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
		scene->cl_data.kernels[1], 1, NULL, &global, &local, 0, NULL, NULL);
}

void			one_argument_sphere(char **description, \
									t_scene *scene, int *snmi)
{
	t_object	*sphere;
	cl_float3	cen_buf[2];
	float		rotation[3];
	float		rad_spec[7];
	t_color		color;

	cen_buf[0] = get_points(description[1]);
	rad_spec[0] = ftoi(get_coordinates(description[2]));
	cen_buf[1] = get_points(description[3]);
	rotation[0] = cen_buf[1].x;
	rotation[1] = cen_buf[1].y;
	rotation[2] = cen_buf[1].z;
	color = get_color(description[4]);
	rad_spec[1] = ftoi(get_coordinates(description[5]));
	rad_spec[2] = ftoi(get_coordinates(description[6]));
	rad_spec[3] = ftoi(get_coordinates(description[7]));
	rad_spec[4] = ftoi(get_coordinates(description[8]));
	rad_spec[5] = ftoi(get_coordinates(description[9]));
	rad_spec[6] = ftoi(get_coordinates(description[10]));
	sphere = new_sphere(cen_buf[0], rad_spec, color, rotation);
	sphere->text = tex_new_bmp(get_file(description[11]));
	sphere->normal_text = tex_new_bmp(get_file(description[12]));
	scene->objs[snmi[1]] = sphere;
	snmi[1]++;
}

t_object		*multiple_spheres(char **description, int i)
{
	t_object	*sphere;
	cl_float3	cen_buf[2];
	float		rotation[3];
	float		rad_spec[7];
	t_color		color;

	cen_buf[0] = get_points(description[i + 1]);
	rad_spec[0] = ftoi(get_coordinates(description[i + 2]));
	cen_buf[1] = get_points(description[i + 3]);
	rotation[0] = cen_buf[1].x;
	rotation[1] = cen_buf[1].y;
	rotation[2] = cen_buf[1].z;
	color = get_color(description[i + 4]);
	rad_spec[1] = ftoi(get_coordinates(description[i + 5]));
	rad_spec[2] = ftoi(get_coordinates(description[i + 6]));
	rad_spec[3] = ftoi(get_coordinates(description[i + 7]));
	rad_spec[4] = ftoi(get_coordinates(description[i + 8]));
	rad_spec[5] = ftoi(get_coordinates(description[i + 9]));
	rad_spec[6] = ftoi(get_coordinates(description[i + 10]));
	sphere = new_sphere(cen_buf[0], rad_spec, color, rotation);
	return (sphere);
}
