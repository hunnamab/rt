/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 22:45:20 by pmetron           #+#    #+#             */
/*   Updated: 2021/01/13 18:59:44 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

t_object	*new_sphere(cl_float3 center, float *rad_spec, t_color color, \
						float *rotation)
{
	t_sphere *new_sphere;
	t_object *new_object;

	new_object = protected_malloc(sizeof(t_object), 1);
	new_sphere = protected_malloc(sizeof(t_sphere), 1);
	new_sphere->center = center;
	new_sphere->radius = rad_spec[0];
	new_object->rotation[0] = rotation[0];
	new_object->rotation[1] = rotation[1];
	new_object->rotation[2] = rotation[2];
	new_object->specular = rad_spec[1];
	new_object->color = color;
	new_object->text = NULL;
	new_object->data = (void *)new_sphere;
	new_object->tag = "sphere";
	new_object->type = SPHERE;
	new_object->intersect = &intersect_ray_sphere;
	new_object->get_normal = &get_sphere_normal;
	new_object->clear_obj = &clear_default;
	new_object->cs_nmb = 0; //cutting surfaces init
	new_object->cutting_surfaces = NULL;
	printf("new sphere radius %f\n", new_sphere->radius);
	return (new_object);
}

void		get_sphere_normal(t_scene *scene, int index, int obj_num)
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

/* float		intersect_ray_sphere(t_scene *scene, int index, cl_float3 *start, cl_float3 *dir)
{
	float		a;
	float		b;
	float		c;
	cl_float3	dist;
	t_sphere	*s;
	
	s = (t_sphere *)scene->objs[index]->data;
	a = vector_dot(dir, dir);
	dist = vector_sub(start, &s->center);
	b = 2 * vector_dot(&dist, dir);
	c = vector_dot(&dist, &dist) - (s->radius * s->radius);
	c = b * b - 4 * a * c;
	if (c >= 0)
	{
		c = sqrt(c);
		return (choose_t((-b + c) / (2 * a), (-b - c) / (2 * a)));
	}
	return (0);
} */

void		intersect_ray_sphere(t_scene *scene, int index)
{
	size_t global = WID * HEI;
	size_t local;
	cl_mem cs;
	if (scene->objs[index]->cs_nmb > 0)
	{
		printf("objs.cs.type == %d\n", scene->objs[0][0].cutting_surfaces[0].type);
		printf("sizeof(t_cs) == %lu\n", sizeof(t_cutting_surface));
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb, scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
	clSetKernelArg(scene->cl_data.kernels[1], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 1, sizeof(cl_mem), &scene->cl_data.scene.camera);
	clSetKernelArg(scene->cl_data.kernels[1], 2, sizeof(t_sphere), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[1], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[1], 6, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[1], 7, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
    clGetKernelWorkGroupInfo(scene->cl_data.kernels[1], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	printf("sphere local == max work group size == %ld\n", local);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[1], 1, NULL, &global, &local, 0, NULL, NULL);
}


void	one_argument_sphere(char **description, t_scene *scene, int *snmi)
{
	t_object	*sphere;
	cl_float3	cen_buf[2];
	float		rotation[3];
	float		rad_spec[2];
	t_color		color;

	cen_buf[0] = get_points(description[1]);
	rad_spec[0] = ftoi(get_coordinates(description[2]));
	cen_buf[1] = get_points(description[3]);
	rotation[0] = cen_buf[1].x;
	rotation[1] = cen_buf[1].y;
	rotation[2] = cen_buf[1].z;
	color = get_color(description[4]);
	rad_spec[1] = ftoi(get_coordinates(description[5]));
	sphere = new_sphere(cen_buf[0], rad_spec, color, rotation);
	sphere->text = tex_new_bmp(get_file(description[6]));
	scene->objs[snmi[1]] = sphere;
	snmi[1]++;
}

t_object 	*multiple_spheres(char **description, t_scene *scene, int *snmi, int i)
{
	t_object	*sphere;
	cl_float3	cen_buf[2];
	float		rotation[3];
	float		rad_spec[2];
	t_color 	color;

	cen_buf[0] = get_points(description[i + 1]);
	rad_spec[0] = ftoi(get_coordinates(description[i + 2]));
	cen_buf[1] = get_points(description[i + 3]);
	rotation[0] = cen_buf[1].x;
	rotation[1] = cen_buf[1].y;
	rotation[2] = cen_buf[1].z;
	color = get_color(description[i + 4]);
	rad_spec[1] = ftoi(get_coordinates(description[i + 5]));
	sphere = new_sphere(cen_buf[0], rad_spec, color, rotation);
	return (sphere);
}
