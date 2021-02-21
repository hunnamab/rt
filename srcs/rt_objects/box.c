/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:44:57 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/21 20:49:46 by npetrell         ###   ########.fr       */
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
	specular[7] = snmi[1];
	new_box(cen_buf, color, specular, scene);
	snmi[1] += 6;
}

void	multiple_boxes(char **description, int i, t_scene *scene, int *snmi)
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
	specular[7] = snmi[1];
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
				multiple_boxes(description, i, scene, snmi);
				snmi[1] += 6;
				/*box->text = tex_new_bmp(get_file(description[i + 10]));
				box->normal_text = tex_new_bmp(get_file(description[i + 11]));*/
				i += 13;
			}
		}
	}
	else if (description[0][0] == '{')
		one_argument_box(description, scene, snmi);
	else
		output_error(6);
}

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
	for(int p = 0; p < 8; p++)
		printf("%d == %f\n", p, parameters[p]);
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
