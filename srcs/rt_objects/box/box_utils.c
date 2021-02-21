/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 00:26:41 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 00:27:16 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void			get_box(char **description, t_scene *scene, int *snmi)
{
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
				i += 13;
			}
		}
	}
	else if (description[0][0] == '{')
		one_argument_box(description, scene, snmi);
	else
		output_error(6);
}

void			init_srfp2(int (*srfp2)[3])
{
	(*srfp2)[0] = 0;
	(*srfp2)[1] = 0;
	(*srfp2)[2] = -1;
}

void			new_box(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene)
{
	first_plane(position, color, parameters, scene);
	second_plane(position, color, parameters, scene);
	third_plane(position, color, parameters, scene);
	fourth_plane(position, color, parameters, scene);
	fifth_plane(position, color, parameters, scene);
	sixth_plane(position, color, parameters, scene);
}
