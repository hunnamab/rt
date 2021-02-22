/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd12345.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 03:26:40 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 19:03:50 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd1(t_scene *scene)
{
	int		ret1;
	char	*get_ray_arr;
	int		err;
	int		fd1;

	fd1 = open("./kernels/get_ray_arr.cl", O_RDONLY);
	get_ray_arr = protected_malloc(sizeof(char), 256000);
	ret1 = read(fd1, get_ray_arr, 64000);
	get_ray_arr[ret1] = '\0';
	if ((scene->cl_data.programs[0] = \
	clCreateProgramWithSource(scene->cl_data.context, 1, \
							(const char **)&get_ray_arr, NULL, &err)))
		ft_printf("created\n");
	if ((clBuildProgram(scene->cl_data.programs[0], 1, \
				&scene->cl_data.device_id, "-I includes", NULL, &err)))
		ft_printf("built\n");
	if (!(scene->cl_data.kernels[0] = \
	clCreateKernel(scene->cl_data.programs[0], "get_ray_arr", &err)))
		ft_printf("error %d\n", err);
	ft_strdel(&get_ray_arr);
	close(fd1);
}

void	fd2(t_scene *scene)
{
	int		ret2;
	char	*intersect_ray_sphere_cl;
	int		fd2;
	int		err;

	err = 0;
	fd2 = open("./kernels/intersect_ray_sphere_cl.cl", O_RDONLY);
	intersect_ray_sphere_cl = protected_malloc(sizeof(char), 256000);
	ret2 = read(fd2, intersect_ray_sphere_cl, 64000);
	intersect_ray_sphere_cl[ret2] = '\0';
	if ((scene->cl_data.programs[1] = \
		clCreateProgramWithSource(scene->cl_data.context, 1, (\
					const char **)&intersect_ray_sphere_cl, NULL, &err)))
		ft_printf("cоздана программа sphere\n");
	if ((clBuildProgram(scene->cl_data.programs[1], 0, NULL, \
											"-I includes", NULL, &err)))
		ft_printf("собрана программа sphere err == %d\n", err);
	if (!(scene->cl_data.kernels[1] = \
		clCreateKernel(scene->cl_data.programs[1], \
						"intersect_ray_sphere_cl", &err)))
		ft_printf("не собрана программа 1, error %d sphere\n", err);
	ft_strdel(&intersect_ray_sphere_cl);
	close(fd2);
}

void	fd3(t_scene *scene)
{
	int		ret3;
	char	*intersect_ray_cone_cl;
	int		fd3;
	int		err;

	err = 0;
	fd3 = open("./kernels/intersect_ray_cone_cl.cl", O_RDONLY);
	intersect_ray_cone_cl = protected_malloc(sizeof(char), 256000);
	ret3 = read(fd3, intersect_ray_cone_cl, 64000);
	intersect_ray_cone_cl[ret3] = '\0';
	if ((scene->cl_data.programs[2] = \
			clCreateProgramWithSource(scene->cl_data.context, 1, \
						(const char **)&intersect_ray_cone_cl, NULL, &err)))
		ft_printf("cоздана программа cone\n");
	if ((clBuildProgram(scene->cl_data.programs[2], 0, NULL, \
												"-I includes", NULL, &err)))
		ft_printf("собрана программа cone\n");
	if (!(scene->cl_data.kernels[2] = \
clCreateKernel(scene->cl_data.programs[2], "intersect_ray_cone_cl", &err)))
		ft_printf("не собрана программа 1, error %d cone\n", err);
	ft_strdel(&intersect_ray_cone_cl);
	close(fd3);
}

void	fd4(t_scene *scene)
{
	int		err;
	int		ret4;
	char	*intersect_ray_cylinder_cl;
	int		fd4;

	err = 0;
	fd4 = open("./kernels/intersect_ray_cylinder_cl.cl", O_RDONLY);
	intersect_ray_cylinder_cl = protected_malloc(sizeof(char), 256000);
	ret4 = read(fd4, intersect_ray_cylinder_cl, 64000);
	intersect_ray_cylinder_cl[ret4] = '\0';
	if ((scene->cl_data.programs[3] = \
clCreateProgramWithSource(scene->cl_data.context, 1, \
(const char **)&intersect_ray_cylinder_cl, NULL, &err)))
		ft_printf("cоздана программа cylinder\n");
	if ((clBuildProgram(scene->cl_data.programs[3], 0, \
									NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа cylinder\n");
	if (!(scene->cl_data.kernels[3] = \
clCreateKernel(scene->cl_data.programs[3], "intersect_ray_cylinder_cl", &err)))
		ft_printf("не собрана программа 1, error %d cylinder\n", err);
	ft_strdel(&intersect_ray_cylinder_cl);
	close(fd4);
}

void	fd5(t_scene *scene)
{
	int		err;
	int		ret5;
	char	*intersect_ray_triangle_cl;
	int		fd5;

	err = 0;
	fd5 = open("./kernels/intersect_ray_triangle_cl.cl", O_RDONLY);
	intersect_ray_triangle_cl = protected_malloc(sizeof(char), 256000);
	ret5 = read(fd5, intersect_ray_triangle_cl, 64000);
	intersect_ray_triangle_cl[ret5] = '\0';
	if ((scene->cl_data.programs[4] = \
	clCreateProgramWithSource(scene->cl_data.context, 1, \
	(const char **)&intersect_ray_triangle_cl, NULL, &err)))
		ft_printf("cоздана программа triangle\n");
	if ((clBuildProgram(scene->cl_data.programs[4], 0, \
							NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа triangle\n");
	if (!(scene->cl_data.kernels[4] = \
clCreateKernel(scene->cl_data.programs[4], "intersect_ray_triangle_cl", &err)))
		ft_printf("не собрана программа 1, error %d triangle\n", err);
	ft_strdel(&intersect_ray_triangle_cl);
	close(fd5);
}
