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

#ifndef RTV1_H
# define RTV1_H

# include "SDL2.framework/Headers/SDL.h" //for macOS
//#include <SDL2/SDL.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdint.h>
# include "libft.h"
# include <unistd.h>
# include "matrix_lib/matrix.h"
# include "types.h"

# define ABS(x)		(x) > 0 ? (x) : -(x)
# define DROUND(d)	ABS(d) < 0.00001 ? 0 : (d)

# define WID 1280
# define HEI 720

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
t_color		reflection_color(t_scene *scene, int index);
t_light		*new_light(t_point *pos_dir, const char *type, double intensity);
t_point		get_light_vec(t_scene *scene, int index, int j);
double		get_specular(t_scene *scene, int index, int j, t_point *l);
int			in_shadow(t_scene *scene, int index, t_point l);
// vector.c
t_point		vector_add(const t_point *v1, const t_point *v2);
t_point		vector_scale(t_point *v, double c);
t_point		vector_sub(t_point *v1, t_point *v2);
double		vector_dot(t_point *v1, t_point *v2);
t_point		vector_cross(t_point *v1, t_point *v2);
double		vector_length(t_point *vector);
t_point		vector_div_by_scalar(t_point *vector, double scalar);
t_point		vector_sub_by_scalar(t_point *vector, double scalar);
void		normalize_vector(t_point *v1);
t_point		vector_add_scalar(const t_point *v1, const double scalar);
// utils.c
t_point		get_point(double x, double y, double z);
void		get_viewport(t_scene *scene);
void		copy_point(t_point *dst, t_point *src);
double		choose_t(double t0, double t1);
void		clear_default(t_object *obj);
void		*protected_malloc(unsigned int size, unsigned int nmb);
// color.c
void		copy_color(t_color *dst, t_color *src);
t_color		set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void		set_color_zero(t_color *color);
t_color		color_mul_by_scalar(t_color *color, double scalar);
// sphere.c
double		intersect_ray_sphere(t_ray *r, t_object *object);
t_object	*new_sphere(t_point center, double *rad_spec, t_color color, \
							double *rotation);
// triangle.c
double		intersect_ray_triangle(t_ray *r, t_object *object);
t_object	*new_triangle(t_point *vertex, double specular, t_color color, \
							double *rotation);
void		clear_triangle(t_object *obj);
// plane.c
double		intersect_ray_plane(t_ray *r, t_object *object);
t_object	*new_plane(t_point *poi_nor, double specular, t_color color, \
						double *rotation);
// cylinder.c
double		intersect_ray_cylinder(t_ray *r, t_object *object);
t_object	*new_cylinder(t_point *pos_vec, double *rad_spec, t_color color, \
							double *rotation);
// cone.c
double		intersect_ray_cone(t_ray *r, t_object *object);
t_object	*new_cone(t_point *pos_vec, double *ang_spec, t_color color, \
						double *rotation);
// ftoi.c
double		ftoi(char *str);
// objects_parameters.c
t_object	*get_sphere(char **description);
t_object	*get_triangle(char **description, double specular);
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
t_point		get_points(char *description);
// transform.c
double		**get_transform_matrix(double *angle, double *move, double *scale);
double		**get_rotation_matrix(double *angle);
double		*get_three_doubles(double x, double y, double z);
void		transform(t_point *point, double **matrix, int point_nmb);
// transform_matrix.c
double		**get_translate_matrix(double *move);
double		**get_scale_matrix(double *scale);
double		**rotate_x(double x);
double		**rotate_z(double z);
double		**rotate_y(double y);
// get_structures.c
t_object	**get_objects_structures(int obj_nmb, t_object **buf);
t_light		**get_light_structures(int light_nmb, t_object **buf);
// buffers.c
void		get_rays_arr(t_scene *scene);
void		get_buffers(t_scene *scene);
void		get_normal_buf(t_scene *scene);
void		get_intersection_buf(t_scene *scene);
void		get_closest_points(t_scene *scene, double t);
void		get_material_buf(t_scene *scene);
// normal.c
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

#endif
