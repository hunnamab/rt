/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:45:06 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/17 22:45:23 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	one_argument_ellipsoid(char **description, t_scene *scene, int *snmi)
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

t_object 	*multiple_ellipsoids(char **description, int i)
{
	t_object	*ellipsoid;
	cl_float3	cen_buf[2];
	float		specular[7];
	t_color 	color;
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


t_object    *new_ellipsoid(cl_float3 *buf, t_color color, float *specular, int surface_id)
{
    t_ellipsoid *el;
	t_object	*new_object;
	float		**matrix;

	new_object = malloc(sizeof(t_object));
	el = malloc(sizeof(t_ellipsoid));
	el->center = buf[0]; 
	el->a = specular[0];
	el->b = specular[1];
	el->c = specular[2];
	new_object->rotation[0] = buf[1].x;
	new_object->rotation[1] = buf[1].y;
	new_object->rotation[2] = buf[1].z;
	new_object->position = buf[0];
	new_object->specular = specular[3];
	new_object->reflection = specular[4];
	new_object->refraction = specular[5];
	new_object->transparency = specular[6];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->data = (void *)el;
	new_object->type = ELLIPSOID;
	new_object->tag = "ellipsoid";
	new_object->intersect = &intersect_ray_ellipsoid;
	new_object->clear_obj = &clear_default;
	return (new_object);
}

void	get_ellipsoid(char **description, t_scene *scene, int *snmi)
{
	t_object	*ellipsoid;
	int i;

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

void        intersect_ray_ellipsoid(t_scene *scene, int index, int is_refractive)
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
	clSetKernelArg(scene->cl_data.kernels[8], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
    clSetKernelArg(scene->cl_data.kernels[8], 2, sizeof(t_ellipsoid), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[8], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[8], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[8], 6, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[8], 7, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[8], 8, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[8], 9, sizeof(cl_mem), &scene->cl_data.scene.material_buf);

    clGetKernelWorkGroupInfo(scene->cl_data.kernels[8], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	ft_printf("ellipsoid local == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[8], 1, NULL, &global, &local, 0, NULL, NULL);
}
