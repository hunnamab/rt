/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:34 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/11 12:08:18 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_HOST_CUH
# define RT_HOST_CUH

# include <cuda_runtime_api.h>
# include <vector_types.h>
# include <cuda.h>
# include <SDL2/SDL.h> //for linux
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdint.h>
# include <unistd.h>
# include "types.h"
# include "device_launch_parameters.h"
# include "libft.h"
# include "matrix.h"
# define ABS(x)		(x) > 0 ? (x) : -(x)
# define DROUND(d)	ABS(d) < 0.00001 ? 0 : (d)
# define WID 1280
# define HEI 720
# define KERNEL_NUM 10
// scenes_reader.c
void		read_scene(int fd, t_scene *scene);
// scenes_reader.c
void		split_objects(int len, t_scene *scene, char *buf);
int			count_objects(int len, char *buf);
char		**get_description(char *scene, int i);
// define_object.c
t_object	*get_parameters(char *name, char **description);
// settings.c
void		sphere_settings(t_sphere *s, t_ray *r);
// draw.c
void		draw_scene(t_sdl *sdl, t_scene *scene);
void		draw_normal_buf(t_sdl *sdl, t_scene *scene);
void		draw_deepth_buf(t_sdl *sdl, t_scene *scene);
void		draw_raycast(t_sdl *sdl, t_scene *scene);
// light.c
t_light		*new_light(float3 *pos_dir, int type, float intensity);
float3	get_light_vec(t_scene *scene, int index, int j);
float		get_specular(t_scene *scene, int index, int j, float3 *l);
// vector.c
float3	vector_add(const float3 *v1, const float3 *v2);
float3	vector_scale(float3 *v, float c);
float3	vector_sub(float3 *v1, float3 *v2);
float		vector_dot(float3 *v1, float3 *v2);
float3	vector_cross(float3 *v1, float3 *v2);
float		vector_length(float3 *vector);
float3	vector_div_by_scalar(float3 *vector, float scalar);
float3	vector_sub_by_scalar(float3 *vector, float scalar);
void		normalize_vector(float3 *v1);
float3	vector_add_scalar(const float3 *v1, const float scalar);
// utils.c
void		print_gpu_info(void);
float3		get_point(float x, float y, float z);
void		get_viewport(t_scene *scene);
void		copy_point(float3 *dst, float3 *src);
float		choose_t(float t0, float t1);
void		clear_default(t_object *obj);
void		*protected_malloc(unsigned int size, unsigned int nmb);
// color.c
void		copy_color(t_color *dst, t_color *src);
t_color		set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void		set_color_zero(t_color *color);
// sphere.c
t_object	*new_sphere(float3 center, float *rad_spec, t_color color, \
							float *rotation);
t_object	*new_ellipsoid(float3 *pos_vec, float *abc, t_color color, \
						float *rotation);
t_object	*new_triangle(float3 *vertex, float specular, t_color color, \
							float *rotation);
void		clear_triangle(t_object *obj);
t_object	*new_plane(float3 *poi_nor, float specular, t_color color, \
						float *rotation);
__host__ void	intersect_ray_cylinder(t_scene *scene, int index);
t_object	*new_cylinder(float3 *pos_vec, float *rad_spec, t_color color, \
							float *rotation);
t_object	*new_cone(float3 *pos_vec, float *ang_spec, t_color color, \
						float *rotation);
__host__ void	intersect_ray_triangle(t_scene *scene, int index);
__host__ void	intersect_ray_plane(t_scene *scene, int index);
__host__ void	intersect_ray_cone(t_scene *scene, int index);
__host__ void	intersect_ray_sphere(t_scene *scene, int index);
__host__ void 	intersect_ray_ellipsoid(t_scene *scene, int index);
__host__ void 	intersect_ray_hyperboloid(t_scene *scene, int index);
__host__ void 	intersect_ray_paraboloid(t_scene *scene, int index);
// objects_parameters.c
t_object	*get_sphere(char **description);
t_object	*get_triangle(char **description, float specular);
t_object	*get_plane(char **description);
t_object	*get_cylinder(char **description);
t_object	*get_cone(char **description);
// light_parameters.c
t_light		*get_light(char **description);
// camera_parameters.c
t_camera	get_camera(char **description);
// parameters_utils.c
char		*get_coordinates(char *description);
t_color		get_color(char *description);
float3		get_points(char *description);
// transform.c
float		**get_transform_matrix(float *angle, float *move, float *scale);
float		**get_rotation_matrix(float *angle);
float		*get_three_floats(float x, float y, float z);
void		transform(float3 *point, float **matrix, int point_nmb);
// transform_matrix.c
float		**get_translate_matrix(float *move);
float		**get_scale_matrix(float *scale);
float		**rotate_x(float x);
float		**rotate_z(float z);
float		**rotate_y(float y);
// get_structures.c
t_object	**get_objects_structures(int obj_nmb, t_object **buf);
t_light		**get_light_structures(int light_nmb, t_object **buf);
// buffers.c
void		get_rays_arr(t_scene *scene);
void		get_buffers(t_scene *scene);
void		get_normal_buf(t_scene *scene);
void		get_intersection_buf(t_scene *scene);
void		get_closest_points(t_scene *scene, float t);
void		get_material_buf(t_scene *scene);
void		get_frame_buf(t_scene *scene);
// normal.c
//__device__ void		get_sphere_normal(t_scene *scene, int index, int obj_num);
void		get_sphere_normal(t_scene *scene, int index, int obj_num);
void		get_plane_normal(t_scene *scene, int index, int obj_num);
void		get_triangle_normal(t_scene *scene, int index, int obj_num);
void		get_cone_normal(t_scene *scene, int index, int obj_num);
void		get_cylinder_normal(t_scene *scene, int index, int obj_num);
// scene.c
void		init_scene(t_scene *scene);
void		init_raycast(t_scene *scene);
void		init_default(t_scene *scene);
void		init_deepth(t_scene *scene);
void		refresh_scene(t_scene *scene);
// keyboard.c
void		camera_left(t_scene *scene);
int			keyboard(t_sdl *sdl, t_scene *scene);
void		camera_up(t_scene *scene);
void		camera_right(t_scene *scene);
void		camera_down(t_scene *scene);
// scene_clear.c
void		clean_scene(t_scene *scene);
// errors_management.c
void		output_description(void);
void		output_error(int tag);
int    		cuda_init(t_scene *scene);
void		print_gpu_info(void);
void 		prepare(t_scene *scene, t_sphere *sphere);
void 		prepare_t(t_scene *scene, t_triangle *t);
#endif
