/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd6789_10.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 03:26:40 by baylak            #+#    #+#             */
/*   Updated: 2021/02/22 19:03:39 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fd6(t_scene *scene)
{
	int		err;
	int		ret6;
	char	*intersect_ray_plane_cl;
	int		fd6;

	err = 0;
	fd6 = open("./kernels/intersect_ray_plane_cl.cl", O_RDONLY);
	intersect_ray_plane_cl = protected_malloc(sizeof(char), 256000);
	ret6 = read(fd6, intersect_ray_plane_cl, 64000);
	intersect_ray_plane_cl[ret6] = '\0';
	if ((scene->cl_data.programs[5] = \
	clCreateProgramWithSource(scene->cl_data.context, 1, \
	(const char **)&intersect_ray_plane_cl, NULL, &err)))
		ft_printf("cоздана программа plane\n");
	if ((clBuildProgram(scene->cl_data.programs[5], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа plane\n");
	if (!(scene->cl_data.kernels[5] = \
clCreateKernel(scene->cl_data.programs[5], "intersect_ray_plane_cl", &err)))
		ft_printf("не собрана программа 1, error %d plane\n", err);
	ft_strdel(&intersect_ray_plane_cl);
	close(fd6);
}

void	fd7(t_scene *scene)
{
	int		err;
	int		ret7;
	char	*get_intersection_point;
	int		fd7;

	err = 0;
	fd7 = open("./kernels/get_intersection_point_cl.cl", O_RDONLY);
	get_intersection_point = protected_malloc(sizeof(char), 256000);
	ret7 = read(fd7, get_intersection_point, 64000);
	get_intersection_point[ret7] = '\0';
	if ((scene->cl_data.programs[6] = \
	clCreateProgramWithSource(scene->cl_data.context, 1, \
	(const char **)&get_intersection_point, NULL, &err)))
		ft_printf("cоздана программа get_intersection_point\n");
	if ((clBuildProgram(scene->cl_data.programs[6], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа get_intersection_point\n");
	if (!(scene->cl_data.kernels[6] = \
clCreateKernel(scene->cl_data.programs[6], "get_intersection_point", &err)))
		ft_printf("не собрана программа 1, \
				error %d get_intersection_point\n", err);
	ft_strdel(&get_intersection_point);
	close(fd7);
}

void	fd8(t_scene *scene)
{
	int		err;
	int		ret8;
	char	*get_normal_buf_cl;
	int		fd8;

	err = 0;
	fd8 = open("./kernels/get_normal_buf_cl.cl", O_RDONLY);
	get_normal_buf_cl = protected_malloc(sizeof(char), 256000);
	ret8 = read(fd8, get_normal_buf_cl, 64000);
	get_normal_buf_cl[ret8] = '\0';
	if ((scene->cl_data.programs[7] = \
	clCreateProgramWithSource(scene->cl_data.context, 1, \
	(const char **)&get_normal_buf_cl, NULL, &err)))
		ft_printf("cоздана программа get_normal_buf_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[7], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа get_normal_buf_cl\n");
	if (!(scene->cl_data.kernels[7] = \
clCreateKernel(scene->cl_data.programs[7], "get_normal_buf_cl", &err)))
		ft_printf("не собрана программа 1, \
							error %d get_normal_buf_cl\n", err);
	ft_strdel(&get_normal_buf_cl);
	close(fd8);
}

void	fd9(t_scene *scene)
{
	int		err;
	int		ret9;
	char	*intersect_ray_ellipsoid_cl;
	int		fd9;

	err = 0;
	fd9 = open("./kernels/intersect_ray_ellipsoid_cl.cl", O_RDONLY);
	intersect_ray_ellipsoid_cl = protected_malloc(sizeof(char), 256000);
	ret9 = read(fd9, intersect_ray_ellipsoid_cl, 64000);
	intersect_ray_ellipsoid_cl[ret9] = '\0';
	if ((scene->cl_data.programs[8] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&intersect_ray_ellipsoid_cl, NULL, &err)))
		ft_printf("cоздана программа intersect_ray_ellipsoid_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[8], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа intersect_ray_ellipsoid_cl\n");
	if (!(scene->cl_data.kernels[8] = \
clCreateKernel(scene->cl_data.programs[8], "intersect_ray_ellipsoid", &err)))
		ft_printf("не собрана программа 1, error %d \
						intersect_ray_ellipsoid_cl\n", err);
	ft_strdel(&intersect_ray_ellipsoid_cl);
	close(fd9);
}

void	fd10(t_scene *scene)
{
	int		err;
	int		ret10;
	char	*get_frame_buf_cl;
	int		fd10;

	err = 0;
	fd10 = open("./kernels/get_frame_buf_cl.cl", O_RDONLY);
	get_frame_buf_cl = protected_malloc(sizeof(char), 256000);
	ret10 = read(fd10, get_frame_buf_cl, 64000);
	get_frame_buf_cl[ret10] = '\0';
	if ((scene->cl_data.programs[9] = \
	clCreateProgramWithSource(scene->cl_data.context, \
	1, (const char **)&get_frame_buf_cl, NULL, &err)))
		ft_printf("cоздана программа get_frame_buf_cl\n");
	if ((clBuildProgram(scene->cl_data.programs[9], \
	0, NULL, "-I includes", NULL, &err)))
		ft_printf("собрана программа get_frame_buf_cl\n");
	if (!(scene->cl_data.kernels[9] = \
clCreateKernel(scene->cl_data.programs[9], "get_frame_buf_cl", &err)))
		ft_printf("не собрана программа 1, error %d \
									get_frame_buf_cl\n", err);
	ft_strdel(&get_frame_buf_cl);
	close(fd10);
}
