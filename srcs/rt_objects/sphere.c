/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 22:45:20 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/13 02:40:16 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "types.h"

cl_float3   clvec_rot_x(cl_float3 v, double a)
{
    cl_float3   r;
    r.x = v.x;
    r.y = (float)(v.y * cos(a) - v.z * sin(a));
    r.z = (float)(v.y * sin(a) + v.z * cos(a));
    return (r);
}
cl_float3   clvec_rot_y(cl_float3 v, double a)
{
    cl_float3   r;
    r.x = (float)(v.x * cos(a) + v.z * sin(a));
    r.y = v.y;
    r.z = (float)(v.z * cos(a) - v.x * sin(a));
    return (r);
}
cl_float3   clvec_rot_z(cl_float3 v, double a)
{
    cl_float3   r;
    r.x = (float)(v.x * cos(a) - v.y * sin(a));
    r.y = (float)(v.x * sin(a) + v.y * cos(a));
    r.z = v.z;
    return (r);
}
t_basis get_basis(t_basis basis, float 	*rot)
{
    basis.u = clvec_rot_x(basis.u, (rot[0]));
    basis.u = clvec_rot_y(basis.u, (rot[1]));
    basis.u = clvec_rot_z(basis.u, (rot[2]));
    basis.v = clvec_rot_x(basis.v, (rot[0]));
    basis.v = clvec_rot_y(basis.v, (rot[1]));
    basis.v = clvec_rot_z(basis.v, (rot[2]));
    basis.w = clvec_rot_x(basis.w, (rot[0]));
    basis.w = clvec_rot_y(basis.w, (rot[1]));
    basis.w = clvec_rot_z(basis.w, (rot[2]));
    return (basis);
}
t_basis get_default(t_basis basis)
{
    basis.u = (cl_float3){{0.0, 1.0, 0.0}};
    basis.v = (cl_float3){{1.0, 0.0, 0.0}};
    basis.w = (cl_float3){{0.0, 0.0, 1.0}};
    return (basis);
}

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
	new_object->reflection = rad_spec[2];
	new_object->refraction = rad_spec[3];
	new_object->transparency = rad_spec[4];
	new_object->t_scale = rad_spec[5];
	new_object->color = color;
	new_object->cs_nmb = 0;
	new_object->surface_id = rad_spec[6];
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->data = (void *)new_sphere;
	new_object->tag = "sphere";
	new_object->type = SPHERE;
	new_object->intersect = &intersect_ray_sphere;
	new_object->get_normal = &get_sphere_normal;
	new_object->clear_obj = &clear_default;
	new_object->cs_nmb = 0; //cutting surfaces init
	new_object->cutting_surfaces = NULL;
	new_object->basis = get_default(new_object->basis);
	new_object->basis = get_basis(new_object->basis, new_object->rotation);
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

void		intersect_ray_sphere(t_scene *scene, int index, int is_refractive)
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
	clSetKernelArg(scene->cl_data.kernels[1], 0, sizeof(cl_mem), &scene->cl_data.scene.ray_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 1, sizeof(cl_mem), &scene->cl_data.scene.intersection_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 2, sizeof(t_sphere), scene->objs[index]->data);
	clSetKernelArg(scene->cl_data.kernels[1], 3, sizeof(cl_mem), &scene->cl_data.scene.depth_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 4, sizeof(cl_mem), &scene->cl_data.scene.index_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 5, sizeof(cl_int), (void*)&index);
	clSetKernelArg(scene->cl_data.kernels[1], 6, sizeof(cl_mem), &cs);
	clSetKernelArg(scene->cl_data.kernels[1], 7, sizeof(cl_int), (void*)&scene->objs[index]->cs_nmb);
	clSetKernelArg(scene->cl_data.kernels[1], 8, sizeof(cl_int), (void*)&scene->bounce_cnt);
	clSetKernelArg(scene->cl_data.kernels[1], 9, sizeof(cl_mem), &scene->cl_data.scene.material_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 10, sizeof(cl_int), (void*)&is_refractive);
	clSetKernelArg(scene->cl_data.kernels[1], 11, sizeof(cl_mem), &scene->cl_data.scene.normal_buf);
	clSetKernelArg(scene->cl_data.kernels[1], 12, sizeof(cl_float), (void*)&scene->objs[index]->refraction);
	clSetKernelArg(scene->cl_data.kernels[1], 13, sizeof(cl_mem), &scene->cl_data.scene.exception_buf);
    clGetKernelWorkGroupInfo(scene->cl_data.kernels[1], scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    clEnqueueNDRangeKernel(scene->cl_data.commands, scene->cl_data.kernels[1], 1, NULL, &global, &local, 0, NULL, NULL);
}

void	one_argument_sphere(char **description, t_scene *scene, int *snmi)
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

t_object 	*multiple_spheres(char **description, int i)
{
	t_object	*sphere;
	cl_float3	cen_buf[2];
	float		rotation[3];
	float		rad_spec[7];
	t_color 	color;
	
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
