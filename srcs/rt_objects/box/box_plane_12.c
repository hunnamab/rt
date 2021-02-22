/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_plane_12.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 00:09:10 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 18:54:28 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		first_plane_part2(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	pn[3];

	pn[0] = get_point(position[0].x + parameters[6] / 2, \
										position[0].y, position[0].z);
	pn[1] = get_point(1, 0, 0);
	pn[2] = position[1];
	scene->objs[(int)parameters[7]] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7]]->position = position[0];
	scene->objs[(int)parameters[7]]->cs_nmb = 4;
	scene->objs[(int)parameters[7]]->cutting_surfaces = \
							malloc(sizeof(t_cutting_surface) * 4);
}

void			srpf_by_fuck_norm(cl_float3 *position, \
float *parameters, cl_float3 *srfp)
{
	srfp[0] = get_point(0, 0, -1);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, \
				position[0].y, position[0].z - parameters[6] / 2);
}

void			first_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	srfp[2];
	int			srfp2[3];

	init_srfp2(&srfp2);
	srfp2[2] = -1;
	first_plane_part2(position, color, parameters, scene);
	srfp[0] = get_point(0, 1, 0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, \
				position[0].y + parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7]]->cutting_surfaces[0] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0, -1, 0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, \
				position[0].y - parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7]]->cutting_surfaces[1] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0, 0, 1);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, \
				position[0].y, position[0].z + parameters[6] / 2);
	scene->objs[(int)parameters[7]]->cutting_surfaces[2] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
	srpf_by_fuck_norm(position, parameters, srfp);
	srfp2[2] = 0;
	scene->objs[(int)parameters[7]]->cutting_surfaces[3] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
}

void			second_plane_part2(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	pn[3];

	pn[0] = get_point(position[0].x - parameters[6] / 2, \
								position[0].y, position[0].z);
	pn[1] = get_point(-1, 0, 0);
	pn[2] = position[1];
	scene->objs[(int)parameters[7] + 1] = new_plane(pn, \
											parameters, color);
	scene->objs[(int)parameters[7] + 1]->position = position[0];
	scene->objs[(int)parameters[7] + 1]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces = \
							malloc(sizeof(t_cutting_surface) * 4);
}

void			second_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	srfp[2];
	int			srfp2[3];

	init_srfp2(&srfp2);
	second_plane_part2(position, color, parameters, scene);
	srfp[0] = get_point(0, 1, 0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, \
				position[0].y + parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[0] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0, -1, 0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, \
				position[0].y - parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[1] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0, 0, 1);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, \
				position[0].y, position[0].z + parameters[6] / 2);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[2] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0, 0, -1);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, \
				position[0].y, position[0].z - parameters[6] / 2);
	scene->objs[(int)parameters[7] + 1]->cutting_surfaces[3] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
}
