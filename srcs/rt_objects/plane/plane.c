/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:22:24 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/22 02:09:50 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

t_basis		get_plane_default(t_basis basis, cl_float3 normal)
{
	if (normal.x == 1)
	{
		basis.u = (cl_float3){{1.0, 0.0, 0.0}};
		basis.v = (cl_float3){{0.0, 1.0, 0.0}};
		basis.w = (cl_float3){{0.0, 0.0, 1.0}};
	}
	else if (normal.y == 1)
	{
		basis.u = (cl_float3){{0.0, 1.0, 0.0}};
		basis.v = (cl_float3){{1.0, 0.0, 0.0}};
		basis.w = (cl_float3){{0.0, 0.0, 1.0}};
	}
	else
	{
		basis.u = (cl_float3){{0.0, 0.0, 1.0}};
		basis.v = (cl_float3){{0.0, 1.0, 0.0}};
		basis.w = (cl_float3){{1.0, 0.0, 0.0}};
	}
	return (basis);
}

void		get_plane_normal(t_scene *scene, int index, int obj_num)
{
	t_plane *p;

	p = (t_plane *)scene->objs[obj_num]->data;
	copy_point(&scene->normal_buf[index], &p->normal);
	if (vector_dot(&scene->ray_buf[index], \
		&scene->normal_buf[index]) > 0.0001)
		scene->normal_buf[index] = vector_scale(&scene->normal_buf[index], -1);
}

void		intersect_ray_plane(t_scene *scene, int index, int is_refractive)
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
	set_arg_plane(scene, index);
	set_arg2_plane(scene, index, is_refractive, cs);
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[5], \
	scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, \
	sizeof(local), &local, NULL);
	ft_printf("local == max work group size == %ld\n", local);
	clEnqueueNDRangeKernel(scene->cl_data.commands, \
	scene->cl_data.kernels[5], 1, NULL, &global, &local, 0, NULL, NULL);
	clReleaseMemObject(cs);
}

void		one_argument_plane(char **description, t_scene *scene, int *snmi)
{
	t_object	*plane;
	t_color		color;
	cl_float3	poi_nor_buf[3];
	float		specular[6];

	poi_nor_buf[0] = get_points(description[1]);
	poi_nor_buf[1] = get_points(description[2]);
	poi_nor_buf[2] = get_points(description[3]);
	color = get_color(description[4]);
	specular[0] = ftoi(get_coordinates(description[5]));
	specular[1] = ftoi(get_coordinates(description[6]));
	specular[2] = ftoi(get_coordinates(description[7]));
	specular[3] = ftoi(get_coordinates(description[8]));
	specular[4] = ftoi(get_coordinates(description[9]));
	specular[5] = ftoi(get_coordinates(description[10]));
	plane = new_plane(poi_nor_buf, specular, color);
	plane->text = tex_new_bmp(get_file(description[11]));
	plane->normal_text = tex_new_bmp(get_file(description[12]));
	scene->objs[snmi[1]] = plane;
	snmi[1]++;
}

t_object	*multiple_planes(char **description, int i)
{
	t_object	*plane;
	t_color		color;
	cl_float3	poi_nor_buf[3];
	float		specular[6];

	poi_nor_buf[0] = get_points(description[i + 1]);
	poi_nor_buf[1] = get_points(description[i + 2]);
	poi_nor_buf[2] = get_points(description[i + 3]);
	color = get_color(description[i + 4]);
	specular[0] = ftoi(get_coordinates(description[i + 5]));
	specular[1] = ftoi(get_coordinates(description[i + 6]));
	specular[2] = ftoi(get_coordinates(description[i + 7]));
	specular[3] = ftoi(get_coordinates(description[i + 8]));
	specular[4] = ftoi(get_coordinates(description[i + 9]));
	specular[5] = ftoi(get_coordinates(description[i + 10]));
	plane = new_plane(poi_nor_buf, specular, color);
	return (plane);
}
