/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd16_17_18_19.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 03:26:40 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:10:12 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd16(t_scene *scene)
{
	int		err;
	int		ret16;
	char	*get_refraction_ray_cl;
	int		fd16;

	err = 0;
	fd16 = open("./kernels/get_refraction_ray_cl.cl", O_RDONLY);
	get_refraction_ray_cl = protected_malloc(sizeof(char), 256000);
	ret16 = read(fd16, get_refraction_ray_cl, 64000);
	get_refraction_ray_cl[ret16] = '\0';
	if ((scene->cl_data.programs[15] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&get_refraction_ray_cl, NULL, &err)))
		ft_printf("cоздана программа get_refraction_ray_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[15], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа get_refraction_ray_cl\n");
	if (!(scene->cl_data.kernels[15] = \
clCreateKernel(scene->cl_data.programs[15], "get_refraction_ray_cl", &err)))
		ft_printf("не собрана программа 1, \
				error %d get_refraction_ray_cl\n", err);
	ft_strdel(&get_refraction_ray_cl);
	close(fd16);
}

void	fd17(t_scene *scene)
{
	int		err;
	int		ret17;
	char	*get_reflection_ray_cl;
	int		fd17;

	err = 0;
	fd17 = open("./kernels/get_reflection_ray_cl.cl", O_RDONLY);
	get_reflection_ray_cl = protected_malloc(sizeof(char), 256000);
	ret17 = read(fd17, get_reflection_ray_cl, 64000);
	get_reflection_ray_cl[ret17] = '\0';
	if ((scene->cl_data.programs[16] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&get_reflection_ray_cl, NULL, &err)))
		ft_printf("cоздана программа get_reflection_ray_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[16], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа get_reflection_ray_cl\n");
	if (!(scene->cl_data.kernels[16] = \
		clCreateKernel(scene->cl_data.programs[16], \
						"get_reflection_ray_cl", &err)))
		ft_printf("не собрана программа 1, error %d \
							get_reflection_ray_cl\n", err);
	ft_strdel(&get_reflection_ray_cl);
	close(fd17);
}

void	fd18(t_scene *scene)
{
	int		err;
	int		ret18;
	char	*get_fresnel_coeff_cl;
	int		fd18;

	err = 0;
	fd18 = open("./kernels/get_fresnel_coeff_cl.cl", O_RDONLY);
	get_fresnel_coeff_cl = protected_malloc(sizeof(char), 256000);
	ret18 = read(fd18, get_fresnel_coeff_cl, 64000);
	get_fresnel_coeff_cl[ret18] = '\0';
	if ((scene->cl_data.programs[17] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&get_fresnel_coeff_cl, NULL, &err)))
		ft_printf("cоздана программа get_fresnel_coeff_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[17], \
				0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа get_fresnel_coeff_cl\n");
	if (!(scene->cl_data.kernels[17] = \
clCreateKernel(scene->cl_data.programs[17], "get_fresnel_coeff_cl", &err)))
		ft_printf("не собрана программа 1, error %d \
									get_fresnel_coeff_cl\n", err);
	ft_strdel(&get_fresnel_coeff_cl);
	close(fd18);
}

void	fd19(t_scene *scene)
{
	int		err;
	int		ret19;
	char	*init_buffers;
	int		fd19;

	err = 0;
	fd19 = open("./kernels/init_buffers.cl", O_RDONLY);
	init_buffers = protected_malloc(sizeof(char), 256000);
	ret19 = read(fd19, init_buffers, 64000);
	init_buffers[ret19] = '\0';
	if ((scene->cl_data.programs[18] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&init_buffers, NULL, &err)))
		ft_printf("cоздана программа init_buffers\n");
	if ((clBuildProgram(scene->cl_data.programs[18], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа init_buffers\n");
	if (!(scene->cl_data.kernels[18] = \
clCreateKernel(scene->cl_data.programs[18], "init_buffers", &err)))
		ft_printf("не собрана программа 1, error %d \
									init_buffers\n", err);
	ft_strdel(&init_buffers);
	close(fd19);
}
