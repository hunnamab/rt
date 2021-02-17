/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:44:57 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/17 23:08:27 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			one_argument_box(char **description, t_scene *scene, int *snmi)
{
	cl_float3	cen_buf[2];
	float		specular[8];
	t_color		color;
	int			surface_id;

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
	specular[7] = ftoi(get_coordinates(description[11]));
	new_box(cen_buf, color, specular, scene);
	/*box->text = tex_new_bmp(get_file(description[12]));
	box->normal_text = tex_new_bmp(get_file(description[13]));
	scene->objs[snmi[1]] = box;
	snmi[1]++;*/
}

t_object		*multiple_boxes(char **description, int i, t_scene *scene)
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
	specular[7] = ftoi(get_coordinates(description[i + 11]));
	new_box(cen_buf, color, specular, scene);
}

void			get_box(char **description, t_scene *scene, int *snmi)
{
	t_object	*box;
	int			i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				box = multiple_boxes(description, i, scene);
				/*box->text = tex_new_bmp(get_file(description[i + 10]));
				box->normal_text = tex_new_bmp(get_file(description[i + 11]));*/
				scene->objs[snmi[1]] = box;
				snmi[1]++;
				i += 12;
			}
		}
	}
	else if (description[0][0] == '{')
		one_argument_box(description, scene, snmi);
	else
		output_error(6);
}
/* t_object		*new_box(cl_float3 *buf, t_color color,
							float *specular, int surface_id)
{
	t_box		*box;
	t_object	*new_object;

	new_object = malloc(sizeof(t_object));
	box = malloc(sizeof(t_box));
	box->a = buf[0];
	box->b = buf[1];
	new_object->rotation[0] = buf[2].x;
	new_object->rotation[1] = buf[2].y;
	new_object->rotation[2] = buf[2].z;
	new_object->specular = specular[0];
	new_object->reflection = specular[1];
	new_object->refraction = specular[2];
	new_object->transparency = specular[3];
	new_object->color = color;
	new_object->text = NULL;
	new_object->normal_text = NULL;
	new_object->data = (void *)box;
	new_object->type = BOX;
	new_object->cs_nmb = 0;
	new_object->surface_id = surface_id;
	new_object->cutting_surfaces = NULL;
	new_object->intersect = &intersect_ray_box;
	new_object->clear_obj = &clear_default;
	return (new_object);
} */
/* так бля, список параметров 0)specular, 1)reflection 2)refraction 3)transperansy 4)t_scale 5)surface_id 
								6)size (это длина стороны куба) 7)objs_id
	 при создании куба нужно знать его id в общем объектном указателе, выделить по этому id сразу 6 объектов
	 для каждой плоскости куба, во все эти объекты добавить необходимые секущие поверхности/негативные фигуры*/
/*void		test_box(t_scene *scene)
{
	cl_float3 pos = get_point(1,2,3);
	t_color color = set_color(0,255,0,255);
	float	parameters[8];
	scene->obj_nmb += 6;
	parameters[0] = 100;
	parameters[1] = 0;
	parameters[2] = 0;
	parameters[3] = 0;
	parameters[4] = 10;
	parameters[5] = 0;
	parameters[6] = 5;
	parameters[7] = 1;
	new_box(pos, color,parameters,scene);
}*/

void		new_box(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	pn[3];
	cl_float3	srfp[2];
	int			srfp2[3];
	srfp2[0] = 0;
	srfp2[1] = 0;
	srfp2[2] = -1;
	srfp2[0] = 0;
	printf("(%f,%f,%f)\n", position[0].x, position[0].y, position[0].z);
	pn[0] = get_point(position[0].x + parameters[6] / 2, position[0].y ,position[0].z);
	pn[1] = get_point(1,0,0);
	pn[2] = get_point(0,0,0);
	scene->objs[(int)parameters[7]] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7]]->position = position[0];
	scene->objs[(int)parameters[7]]->cs_nmb = 4;
	scene->objs[(int)parameters[7]]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y + parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7]]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y - parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7]]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y , position[0].z + parameters[6] / 2);
	scene->objs[(int)parameters[7]]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y, position[0].z - parameters[6] / 2);
	srfp2[2] = 0;
	scene->objs[(int)parameters[7]]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//second plane
	pn[0] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z);
	pn[1] = get_point(-1,0,0);
	scene->objs[(int)parameters[7] + 1] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7] + 1]->position = position[0];
	scene->objs[(int)parameters[7] + 1]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y + parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y - parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z + parameters[6] / 2);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z - parameters[6] / 2);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//third plane
	pn[0] = get_point(position[0].x, position[0].y, position[0].z + parameters[6] / 2);
	pn[1] = get_point(0,0,1);
	scene->objs[(int)parameters[7] + 2] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7] + 2]->position = position[0];
	scene->objs[(int)parameters[7] + 2]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 2]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position[0].x, position[0].y + parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 2]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position[0].x, position[0].y - parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 2]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 2]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 2]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//fourth plane
	pn[0] = get_point(position[0].x, position[0].y + parameters[6] / 2, position[0].z);
	pn[1] = get_point(0,1,0);
	scene->objs[(int)parameters[7] + 3] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7] + 3]->position = position[0];
	scene->objs[(int)parameters[7] + 3]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 3]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y, position[0].z + parameters[6] / 2);
	scene->objs[(int)parameters[7] + 3]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z - parameters[6] / 2);
	scene->objs[(int)parameters[7] + 3]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 3]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 3]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//fifth plane
	pn[0] = get_point(position[0].x, position[0].y - parameters[6] / 2, position[0].z);
	pn[1] = get_point(0,-1,0);
	scene->objs[(int)parameters[7] + 4] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7] + 4]->position = position[0];
	scene->objs[(int)parameters[7] + 4]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y, position[0].z + parameters[6] / 2);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z - parameters[6] / 2);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//last
	pn[0] = get_point(position[0].x, position[0].y, position[0].z - parameters[6] / 2);
	pn[1] = get_point(0,0,-1);
	scene->objs[(int)parameters[7] + 5] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7] + 5]->position = position[0];
	scene->objs[(int)parameters[7] + 5]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position[0].x, position[0].y + parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position[0].x, position[0].y - parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\","); 
}
void			intersect_ray_box(t_scene *scene, int index, int is_refractive)
{
	size_t		global;
	size_t		local;
	cl_mem		cs;

	global = WID * HEI;
	if (scene->objs[index]->cs_nmb > 0)
	{
		cs = clCreateBuffer(scene->cl_data.context, CL_MEM_READ_ONLY |
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
					sizeof(t_cutting_surface) * scene->objs[index]->cs_nmb,
							scene->objs[index]->cutting_surfaces, NULL);
	}
	else
		cs = NULL;
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
	clGetKernelWorkGroupInfo(scene->cl_data.kernels[11],
		scene->cl_data.device_id, CL_KERNEL_WORK_GROUP_SIZE,
				sizeof(local), &local, NULL);
	clEnqueueNDRangeKernel(scene->cl_data.commands,
		scene->cl_data.kernels[11], 1, NULL, &global, &local, 0, NULL, NULL);
}
