/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:22:24 by pmetron           #+#    #+#             */
/*   Updated: 2021/01/29 21:16:35 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

t_object	*new_plane(cl_float3 *poi_nor, float *specular, t_color color, \
						float *rotation)
{
	t_plane		*new_plane;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_plane = protected_malloc(sizeof(t_plane), 1);
	new_plane->normal = poi_nor[1];
	normalize_vector(&new_plane->normal);
	new_plane->point = poi_nor[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_plane->normal, matrix, 1);
	matr_free(matrix, 4);
	new_object->specular = specular[0];
	new_object->reflection = specular[1];
	new_object->cs_nmb = 0;
	new_object->color = color;
	new_object->data = (void *)new_plane;
	new_object->tag = "plane";
	new_object->type = PLANE;
	new_object->text = NULL;
	new_object->intersect = &intersect_ray_plane;
	new_object->get_normal = &get_plane_normal;
	new_plane->d = -new_plane->normal.x * new_plane->point.x - new_plane->\
	normal.y * new_plane->point.y - new_plane->normal.z * new_plane->point.z;
	new_object->clear_obj = &clear_default;
	return (new_object);
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

void		intersect_ray_plane(t_scene *scene, int index)
{
	size_t global = WID * HEI;
	size_t local;
	cl_mem cs;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb, scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	clSetKernelArg(scene->cl_data.kernels[5], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 3, sizeof(t_plane), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[5], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[5], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[5], 6, sizeof(cl_float), (void*)&scene->objs[index]->reflection);
	clSetKernelArg(scene->cl_data.kernels[5], 7, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[5], 8, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[5], 9, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[5], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[5], 1, NULL, &global, &local, 0, NULL, NULL);
	clReleaseMemObject(cs);
}

void	one_argument_plane(char **description, t_scene *scene, int *snmi)
{
	t_object	*plane;
	t_color		color;
	cl_float3	poi_nor_buf[3];
	double		specular[2];
	double		rotation[3];

	poi_nor_buf[0] = get_points(description[1]);
	poi_nor_buf[1] = get_points(description[2]);
	poi_nor_buf[2] = get_points(description[3]);
	rotation[0] = poi_nor_buf[2].x;
	rotation[1] = poi_nor_buf[2].y;
	rotation[2] = poi_nor_buf[2].z;
	color = get_color(description[4]);
	specular[0] = ftoi(get_coordinates(description[5]));
	specular[1] = ftoi(get_coordinates(description[6]));
	plane = new_plane(poi_nor_buf, specular, color, rotation);
	plane->text = tex_new_bmp(get_file(description[7]));
	scene->objs[snmi[1]] = plane;
	snmi[1]++;
}

t_object 	*multiple_planes(char **description, t_scene *scene, int *snmi, int i)
{
	t_object	*plane;
	t_color		color;
	cl_float3	poi_nor_buf[3];
	double		specular[2];
	double		rotation[3];

	poi_nor_buf[0] = get_points(description[i + 1]);
	poi_nor_buf[1] = get_points(description[i + 2]);
	poi_nor_buf[2] = get_points(description[i + 3]);
	rotation[0] = poi_nor_buf[2].x;
	rotation[1] = poi_nor_buf[2].y;
	rotation[2] = poi_nor_buf[2].z;
	color = get_color(description[i + 4]);
	specular[0] = ftoi(get_coordinates(description[i + 5]));
	specular[1] = ftoi(get_coordinates(description[6]));
	plane = new_plane(poi_nor_buf, specular, color, rotation);
	return (plane);
}
