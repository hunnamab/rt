/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:21:11 by pmetron           #+#    #+#             */
/*   Updated: 2020/11/10 17:25:26 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_cuda.h"

void	init_deepth(t_scene *scene)
{
	float	**matrix;
	int		x;

	x = -1;
	scene->viewport = protected_malloc(sizeof(float3), (WID * HEI));
	get_viewport(scene);
	scene->ray_buf = protected_malloc(sizeof(float3), (WID * HEI));
	get_rays_arr(scene);
	scene->depth_buf = protected_malloc(sizeof(float), WID * HEI);
	scene->index_buf = protected_malloc(sizeof(int), WID * HEI);
	scene->frame_buf = protected_malloc(sizeof(t_color), WID * HEI);
	matrix = get_rotation_matrix(scene->camera.rotation);
	while (++x < WID * HEI)
		transform(&scene->ray_buf[x], matrix, 1);
	cudaMemcpy(scene->device_data->ray_buf, scene->ray_buf, sizeof(float3) * WID * HEI, cudaMemcpyHostToDevice);
	matr_free(matrix, 4);
	get_closest_points(scene, 0);
}

void	init_default(t_scene *scene)
{
	float	**matrix;
	int		x;

	x = -1;
	scene->viewport = protected_malloc(sizeof(float3), (WID * HEI));
	get_viewport(scene);
	scene->ray_buf = protected_malloc(sizeof(float3), (WID * HEI));
	get_rays_arr(scene);
	scene->normal_buf = protected_malloc(sizeof(float3), WID * HEI);
	scene->material_buf = protected_malloc(sizeof(t_material), WID * HEI);
	scene->intersection_buf = protected_malloc(sizeof(float3), WID * HEI);
	scene->index_buf = protected_malloc(sizeof(int), WID * HEI);
	scene->depth_buf = protected_malloc(sizeof(float), WID * HEI);
	scene->frame_buf = protected_malloc(sizeof(t_color), WID * HEI);
	matrix = get_rotation_matrix(scene->camera.rotation);
	while (++x < WID * HEI)
		transform(&scene->ray_buf[x], matrix, 1);
	cudaMemcpy(scene->device_data->ray_buf, scene->ray_buf, sizeof(float3) * WID * HEI, cudaMemcpyHostToDevice);
	matr_free(matrix, 4);
	get_closest_points(scene, 0);
	get_intersection_buf(scene);
	get_normal_buf(scene);
	get_material_buf(scene);
	get_frame_buf(scene);
}

void	init_raycast(t_scene *scene)
{
	float	**matrix;
	int		x;

	x = -1;
	scene->viewport = protected_malloc(sizeof(float3), (WID * HEI));
	get_viewport(scene);
	scene->ray_buf = protected_malloc(sizeof(float3), (WID * HEI));
	get_rays_arr(scene);
	scene->depth_buf = protected_malloc(sizeof(float), WID * HEI);
	scene->material_buf = protected_malloc(sizeof(t_material), WID * HEI);
	scene->index_buf = protected_malloc(sizeof(int), WID * HEI);
	scene->intersection_buf = protected_malloc(sizeof(float3), WID * HEI);
	scene->frame_buf = protected_malloc(sizeof(t_color), WID * HEI);
	matrix = get_rotation_matrix(scene->camera.rotation);
	while (++x < WID * HEI)
		transform(&scene->ray_buf[x], matrix, 1);
	cudaMemcpy(scene->device_data->ray_buf, scene->ray_buf, sizeof(float3) * WID * HEI, cudaMemcpyHostToDevice);
	matr_free(matrix, 4);
	get_closest_points(scene, 0);
	get_intersection_buf(scene);
	get_material_buf(scene);
}

void	refresh_scene(t_scene *scene)
{
	float	**matrix;
	int		x;

	x = -1;
	scene->viewport != NULL ? get_viewport(scene) : NULL;
	scene->ray_buf != NULL ? get_rays_arr(scene) : NULL;
	matrix = get_rotation_matrix(scene->camera.rotation);
	while (++x < WID * HEI)
		transform(&scene->ray_buf[x], matrix, 1);
	matr_free(matrix, 4);
	cudaMemcpy(scene->device_data->ray_buf, scene->ray_buf, sizeof(float3) * WID * HEI, cudaMemcpyHostToDevice);
	scene->depth_buf != NULL ? get_closest_points(scene, 0) : NULL;
	scene->intersection_buf != NULL ? get_intersection_buf(scene) : NULL;
	scene->normal_buf != NULL ? get_normal_buf(scene) : NULL;
	scene->material_buf != NULL ? get_material_buf(scene) : NULL;
	scene->frame_buf != NULL ? get_frame_buf(scene) : NULL;
}

void	init_scene(t_scene *scene)
{
	scene->init[0] = &init_default;
	scene->init[1] = &init_default;
	scene->init[2] = &init_deepth;
	scene->init[3] = &init_raycast;
	scene->draw[0] = &draw_scene;
	scene->draw[1] = &draw_normal_buf;
	scene->draw[2] = &draw_deepth_buf;
	scene->draw[3] = &draw_raycast;
	scene->normal_buf = NULL;
	scene->material_buf = NULL;
	scene->intersection_buf = NULL;
	scene->ray_buf = NULL;
	scene->viewport = NULL;
	scene->index_buf = NULL;
	scene->depth_buf = NULL;
	scene->frame_buf = NULL;
	cuda_init(scene);
	scene->init[scene->mode](scene);
}
