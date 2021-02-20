/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surfaces.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 13:48:30 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/20 21:47:34 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	choose_type(char *type)
{
	int lol = 0;

	if (ft_strequ(type, "\"plane\","))
		lol = PLANE;
	else if (ft_strequ(type, "\"sphere\","))
		lol = SPHERE;
	else if (ft_strequ(type, "\"cylinder\","))
		lol = CYLINDER;
	else
		output_error(5);
	return (lol);
}

t_cutting_surface		new_srf(cl_float3 *param, int *obj_neg, cl_float param3, char *type)
{
	t_cutting_surface new_srf;
	new_srf.type = choose_type(type);
	new_srf.object = obj_neg[0];
	new_srf.is_negative = obj_neg[1];
	new_srf.is_local = obj_neg[2];
	new_srf.param1 = param[0];
	new_srf.param2 = param[1];
	new_srf.param3 = param3;
	if(new_srf.type == PLANE)
	{
		new_srf.param3 = -(new_srf.param1.x * new_srf.param2.x) - new_srf.param1.y * new_srf.param2.y - new_srf.param1.z * new_srf.param2.z;
		printf("new_srf d == %f\n", new_srf.param3);
	}
	return (new_srf);
}

void	one_srf(char **description, t_scene *scene, int * snmi)
{
	t_cutting_surface		srf;
	cl_float3	param[2];
	cl_float	param3;
	int			obj_neg[3];
	char		*type;
	
	obj_neg[0] = ft_atoi(get_coordinates(description[1]));
	type = get_light_type(description[2]);
	obj_neg[1] = ft_atoi(get_coordinates(description[3]));
	obj_neg[2] = ft_atoi(get_coordinates(description[4]));
	param[0] = get_points(description[5]);
	param[1] = get_points(description[6]);
	param3 = ftoi(get_coordinates(description[7]));
	srf = new_srf(param, obj_neg, param3, type);
	scene->srfs[snmi[4]] = srf;
	snmi[4]++;
	free(type);
}

t_cutting_surface		many_srfs(char **description, int i)
{
	t_cutting_surface		srf;
	cl_float3	param[2];
	cl_float	param3;
	int			obj_neg[3];
	char		*type;

	obj_neg[0] = ft_atoi(get_coordinates(description[i + 1]));
	type = get_light_type(description[i + 2]);
	obj_neg[1] = ft_atoi(get_coordinates(description[i + 3]));
	obj_neg[2] = ft_atoi(get_coordinates(description[i + 4]));
	param[0] = get_points(description[i + 5]);
	param[1] = get_points(description[i + 6]);
	param3 = ftoi(get_coordinates(description[i + 7]));
	srf = new_srf(param, obj_neg, param3, type);
	free(type);
	return (srf);
}

void	get_surface(char **description, t_scene *scene, int *snmi)
{
	t_cutting_surface		srf;
	int 		i;

	i = 1;
	if (description[0][0] == '[')
	{
		while (description[i][1] != ']')
		{
			if (description[i][2] == '{')
			{
				srf = many_srfs(description, i);
				scene->srfs[snmi[4]] = srf;
				snmi[4]++;
				i += 9;
			}
		}
	}
	else if (description[0][0] == '{')
		one_srf(description, scene, snmi);
	else
		output_error(6);
}
