/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:11:46 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/13 01:38:24 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

t_object	*new_cylinder(cl_float3 *pos_vec, float *rad_spec, t_color color)
{
	t_cylinder	*new_cylinder;
	t_object	*new_object;
	float		**matrix;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_cylinder = protected_malloc(sizeof(t_cylinder), 1);
	new_cylinder->position = pos_vec[0];
	new_cylinder->radius = rad_spec[0];
	new_cylinder->vec = pos_vec[1];
	new_object->specular = rad_spec[1];
	new_object->reflection = rad_spec[2];
	new_object->refraction = rad_spec[3];
	new_object->transparency = rad_spec[4];
	new_object->t_scale = rad_spec[5];
	new_object->rotation[0] = pos_vec[2].x;
	new_object->rotation[1] = pos_vec[2].y;
	new_object->rotation[2] = pos_vec[2].z;
	matrix = get_rotation_matrix(new_object->rotation);
	transform(&new_cylinder->vec, matrix, 1);
	matr_free(matrix, 4);
	new_object->cs_nmb = 0;
	new_object->surface_id = rad_spec[6];
	new_object->cutting_surfaces = NULL;
	new_object->color = color;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->data = (void *)new_cylinder;
	new_object->tag = "cylinder";
	new_object->type = CYLINDER;
	new_object->intersect = &intersect_ray_cylinder;
	new_object->clear_obj = &clear_default;
	new_object->basis = get_default(new_object->basis);
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
	return (new_object);
}

void		intersect_ray_cylinder(t_scene *scene, int index, int is_refractive)
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
	clSetKernelArg(scene->cl_data.kernels[3], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 2, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 3, sizeof(t_cylinder), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[3], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[3], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[3], 6, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[3], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[3], 8, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[3], 9, sizeof(cl_mem), &scene->cl_data.scene.material_buf);
	
    clGetKernelWorkGroupInfo(scene->cl_data.kernels[3], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[3], 1, NULL, &global, &local, 0, NULL, NULL);
}

void	one_argument_cylinder(char **description, t_scene *scene, int *snmi)
{
	t_object	*cylinder;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rotation[3];
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

t_object 	*multiple_cylinders(char **description, int i)
{
	t_object	*cylinder;
	t_color		color;
	cl_float3	pos_vec_buf[3];
	float		rotation[3];
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
