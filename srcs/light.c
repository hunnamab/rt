/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:23:24 by pmetron           #+#    #+#             */
/*   Updated: 2021/02/16 22:18:12 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_light		new_light(cl_float3 *pos_dir, int type, float intensity)
{
	t_light new_light;

	new_light.type = type;
	new_light.intensity = intensity;
	new_light.position = pos_dir[0];
	new_light.direction = pos_dir[1];
	printf("LKNWKNWKNFNKWNKW %f %f %f \n", new_light.position.x, new_light.position.y,new_light.position.z);
	return (new_light);
}