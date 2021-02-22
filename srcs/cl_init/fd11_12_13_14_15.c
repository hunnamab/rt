/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd11_12_13_14_15.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 03:26:40 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 04:03:43 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd11(t_scene *scene)
{
	int		err;
	int		ret11;
	char	*get_material_buf_cl;
	int		fd11;

	err = 0;
	fd11 = open("./kernels/get_material_buf_cl.cl", O_RDONLY);
	get_material_buf_cl = protected_malloc(sizeof(char), 256000);
	ret11 = read(fd11, get_material_buf_cl, 64000);
	get_material_buf_cl[ret11] = '\0';
	if ((scene->cl_data.programs[10] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&get_material_buf_cl, NULL, &err)))
		ft_printf("cоздана программа get_material_buf_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[10], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа get_material_buf_cl\n");
	if (!(scene->cl_data.kernels[10] = \
clCreateKernel(scene->cl_data.programs[10], "get_material_buf_cl", &err)))
		ft_printf("не собрана программа 10, error %d \
							get_material_buf_cl\n", err);
	ft_strdel(&get_material_buf_cl);
	close(fd11);
}

void	fd12(t_scene *scene)
{
	int		err;
	int		ret12;
	char	*intersect_ray_box_cl;
	int		fd12;

	err = 0;
	fd12 = open("./kernels/intersect_ray_box_cl.cl", O_RDONLY);
	intersect_ray_box_cl = protected_malloc(sizeof(char), 256000);
	ret12 = read(fd12, intersect_ray_box_cl, 64000);
	intersect_ray_box_cl[ret12] = '\0';
	if ((scene->cl_data.programs[11] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&intersect_ray_box_cl, NULL, &err)))
		ft_printf("cоздана программа intersect_ray_box_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[11], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа intersect_ray_box_cl\n");
	if (!(scene->cl_data.kernels[11] = \
clCreateKernel(scene->cl_data.programs[11], "intersect_ray_box", &err)))
		ft_printf("не собрана программа 1, error %d \
							intersect_ray_box_cl\n", err);
	ft_strdel(&intersect_ray_box_cl);
	close(fd12);
}

void	fd13(t_scene *scene)
{
	int		err;
	int		ret13;
	char	*intersect_ray_paraboloid_cl;
	int		fd13;

	err = 0;
	fd13 = open("./kernels/intersect_ray_paraboloid_cl.cl", O_RDONLY);
	intersect_ray_paraboloid_cl = protected_malloc(sizeof(char), 256000);
	ret13 = read(fd13, intersect_ray_paraboloid_cl, 64000);
	intersect_ray_paraboloid_cl[ret13] = '\0';
	if ((scene->cl_data.programs[12] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&intersect_ray_paraboloid_cl, NULL, &err)))
		ft_printf("cоздана программа intersect_ray_paraboloid_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[12], \
						0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа intersect_ray_paraboloid_cl\n");
	if (!(scene->cl_data.kernels[12] = \
			clCreateKernel(scene->cl_data.programs[12], \
							"intersect_ray_paraboloid", &err)))
		ft_printf("не собрана программа 1, error %d \
					intersect_ray_paraboloid_cl\n", err);
	ft_strdel(&intersect_ray_paraboloid_cl);
	close(fd13);
}

void	fd14(t_scene *scene)
{
	int		err;
	int		ret14;
	char	*intersect_ray_torus_cl;
	int		fd14;

	err = 0;
	fd14 = open("./kernels/intersect_ray_torus_cl.cl", O_RDONLY);
	intersect_ray_torus_cl = protected_malloc(sizeof(char), 256000);
	ret14 = read(fd14, intersect_ray_torus_cl, 64000);
	intersect_ray_torus_cl[ret14] = '\0';
	if ((scene->cl_data.programs[13] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&intersect_ray_torus_cl, NULL, &err)))
		ft_printf("cоздана программа intersect_ray_torus_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[13], \
					0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа intersect_ray_torus_cl\n");
	if (!(scene->cl_data.kernels[13] = \
clCreateKernel(scene->cl_data.programs[13], "intersect_ray_torus", &err)))
		ft_printf("не собрана программа 1, \
					error %d intersect_ray_torus_cl\n", err);
	ft_strdel(&intersect_ray_torus_cl);
	close(fd14);
}

void	fd15(t_scene *scene)
{
	int		err;
	int		ret15;
	char	*intersect_ray_hyperboloid_cl;
	int		fd15;

	err = 0;
	fd15 = open("./kernels/intersect_ray_hyperboloid_cl.cl", O_RDONLY);
	intersect_ray_hyperboloid_cl = protected_malloc(sizeof(char), 256000);
	ret15 = read(fd15, intersect_ray_hyperboloid_cl, 64000);
	intersect_ray_hyperboloid_cl[ret15] = '\0';
	if ((scene->cl_data.programs[14] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&intersect_ray_hyperboloid_cl, NULL, &err)))
		ft_printf("cоздана программа intersect_ray_hyperboloid_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[14], \
						0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа intersect_ray_hyperboloid_cl\n");
	if (!(scene->cl_data.kernels[14] = \
		clCreateKernel(scene->cl_data.programs[14], \
							"intersect_ray_hyperboloid", &err)))
		ft_printf("не собрана программа 1, error %d \
							intersect_ray_hyperboloid_cl\n", err);
	ft_strdel(&intersect_ray_hyperboloid_cl);
	close(fd15);
}
