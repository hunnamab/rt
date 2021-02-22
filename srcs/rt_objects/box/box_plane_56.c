/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_plane_56.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 00:24:46 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 18:46:13 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			fifth_plane_part2(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	pn[3];

	pn[0] = get_point(position[0].x, position[0].y - \
							parameters[6] / 2, position[0].z);
	pn[1] = get_point(0, -1, 0);
	pn[2] = position[1];
	scene->objs[(int)parameters[7] + 4] = new_plane(pn, \
											parameters, color);
	scene->objs[(int)parameters[7] + 4]->position = position[0];
	scene->objs[(int)parameters[7] + 4]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces = \
							malloc(sizeof(t_cutting_surface) * 4);
}

void			fifth_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	srfp[2];
	int			srfp2[3];

	init_srfp2(&srfp2);
	fifth_plane_part2(position, color, parameters, scene);
	srfp[0] = get_point(0, 0, 1);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, \
			position[0].y, position[0].z + parameters[6] / 2);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[0] = \
						new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0, 0, -1);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, \
		position[0].y, position[0].z - parameters[6] / 2);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[1] = \
					new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1, 0, 0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, \
								position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[2] = \
							new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1, 0, 0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, \
							position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 4]->cutting_surfaces[3] = \
						new_srf(srfp, srfp2, 0, "\"plane\",");
}

void			sixth_plane_part2(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	pn[3];

	pn[0] = get_point(position[0].x, position[0].y, \
									position[0].z - parameters[6] / 2);
	pn[1] = get_point(0, 0, -1);
	pn[2] = position[1];
	scene->objs[(int)parameters[7] + 5] = new_plane(pn, parameters, color);
	scene->objs[(int)parameters[7] + 5]->position = position[0];
	scene->objs[(int)parameters[7] + 5]->cs_nmb = 4;
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces = \
							malloc(sizeof(t_cutting_surface) * 4);
}

void			sixth_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	cl_float3	srfp[2];
	int			srfp2[3];

	init_srfp2(&srfp2);
	sixth_plane_part2(position, color, parameters, scene);
	srfp[0] = get_point(0, 1, 0);
	srfp[1] = get_point(position[0].x, position[0].y + \
								parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[0] = \
								new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0, -1, 0);
	srfp[1] = get_point(position[0].x, position[0].y - \
									parameters[6] / 2, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[1] = \
								new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1, 0, 0);
	srfp[1] = get_point(position[0].x + parameters[6] / 2, \
										position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[2] = \
								new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1, 0, 0);
	srfp[1] = get_point(position[0].x - parameters[6] / 2, \
										position[0].y, position[0].z);
	scene->objs[(int)parameters[7] + 5]->cutting_surfaces[3] = \
								new_srf(srfp, srfp2, 0, "\"plane\",");
}
