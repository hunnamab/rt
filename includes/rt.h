/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:34 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/26 19:57:38 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

#ifdef __APPLE__
	# include <OpenCL/opencl.h> //for macOS
	# include "../frameworks/SDL2.framework/Headers/SDL.h" //for macOS
	# include "../frameworks/SDL_image/SDL_image.h"
	# include "../frameworks/SDL_mixer/SDL_mixer.h"
#else
	# include <SDL2/SDL.h> //for linux
	# include <CL/cl.h> //for linux
#endif
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdint.h>
# include "libft.h"
# include <unistd.h>
# include "../matrix_lib/matrix.h"
# include "types.h"
# include "filters.h"

# define ABS(x)		(x) > 0 ? (x) : -(x)
# define DROUND(d)	ABS(d) < 0.00001 ? 0 : (d)
# define DTR(k)	((double)k * (M_PI / 180))

# define WID 1280
# define HEI 720
# define KERNEL_NUM 20

// scenes_reader.c
void		read_scene(int fd, t_scene *scene);
// scenes_reader.c
void		split_objects(int len, t_scene *scene, char *buf);
int			count_objects(int len, char *buf);
char		**get_description(char *scene, int i);
// define_object.c
void		get_parameters(char *name, char **description, t_scene *scene, int *snmi);
// settings.c
void		sphere_settings(t_sphere *s, t_ray *r);
// draw.c
void		draw_scene(t_sdl *sdl, t_scene *scene);
void		draw_normal_buf(t_sdl *sdl, t_scene *scene);
void		draw_deepth_buf(t_sdl *sdl, t_scene *scene);
void		draw_raycast(t_sdl *sdl, t_scene *scene);
// light.c
t_color		reflection_color(t_scene *scene, int index);
t_light		new_light(cl_float3 *pos_dir, int type, float intensity);
cl_float3	get_light_vec(t_scene *scene, int index, int j);
float		get_specular(t_scene *scene, int index, int j, cl_float3 *l);
int			in_shadow(t_scene *scene, int index, cl_float3 l);
// vector.c
cl_float3	vector_add(const cl_float3 *v1, const cl_float3 *v2);
cl_float3	vector_scale(cl_float3 *v, float c);
cl_float3	vector_sub(cl_float3 *v1, cl_float3 *v2);
float		vector_dot(cl_float3 *v1, cl_float3 *v2);
cl_float3	vector_cross(cl_float3 *v1, cl_float3 *v2);
float		vector_length(cl_float3 *vector);
cl_float3	vector_div_by_scalar(cl_float3 *vector, float scalar);
cl_float3	vector_sub_by_scalar(cl_float3 *vector, float scalar);
void		normalize_vector(cl_float3 *v1);
cl_float3	vector_add_scalar(const cl_float3 *v1, const float scalar);
// utils.c
cl_float3	get_point(float x, float y, float z);
void		get_viewport(t_scene *scene);
void		copy_point(cl_float3 *dst, cl_float3 *src);
float		choose_t(float t0, float t1);
void		clear_default(t_object *obj);
void		*protected_malloc(unsigned int size, unsigned int nmb);
// color.c
void		copy_color(t_color *dst, t_color *src);
t_color		set_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
void		set_color_zero(t_color *color);
t_color		color_mul_by_scalar(t_color *color, float scalar);
// sphere.c
void		intersect_ray_sphere(t_scene *scene, int index);
t_object	*new_sphere(cl_float3 center, float *rad_spec, t_color color, \
							float *rotation);
// triangle.c
void		intersect_ray_triangle(t_scene *scene, int index);
t_object	*new_triangle(cl_float3 *vertex, float specular, t_color color, \
							float *rotation);
void		clear_triangle(t_object *obj);
// plane.c
void		intersect_ray_plane(t_scene *scene, int index);
t_object	*new_plane(cl_float3 *poi_nor, float specular, t_color color, \
						float *rotation);
// cylinder.c
void		intersect_ray_cylinder(t_scene *scene, int index);
t_object	*new_cylinder(cl_float3 *pos_vec, float *rad_spec, t_color color, \
							float *rotation);
// cone.c
void		intersect_ray_cone(t_scene *scene, int index);
t_object	*new_cone(cl_float3 *pos_vec, float *ang_spec, t_color color, \
						float *rotation);
						
void		get_sphere(char **description, t_scene *scene, int *snmi);
void		get_triangle(char **description, float specular, t_scene *scene, int *snmi);
void		get_plane(char **description, t_scene *scene, int *snmi);
void		get_cylinder(char **description, t_scene *scene, int *snmi);
void		get_cone(char **description, t_scene *scene, int *snmi);
// light_parameters.c
void		get_light(char **description, t_scene *scene, int *snmi);
// camera_parameters.c
void		get_camera(char **description, t_scene *scene);
// parameters_utils.c
char		*get_coordinates(char *description);
t_color		get_color(char *description);
cl_float3	get_points(char *description);
// transform.c
float		**get_transform_matrix(float *angle, float *move, float *scale);
float		**get_rotation_matrix(float *angle);
float		*get_three_floats(float x, float y, float z);
void		transform(cl_float3 *point, float **matrix, int point_nmb);
// transform_matrix.c
float		**get_translate_matrix(float *move);
float		**get_scale_matrix(float *scale);
float		**rotate_x(float x);
float		**rotate_z(float z);
float		**rotate_y(float y);
// buffers.c
void		get_rays_arr(t_scene *scene);
void		get_buffers(t_scene *scene);
void		get_normal_buf(t_scene *scene);
void		get_intersection_buf(t_scene *scene);
void		get_closest_points(t_scene *scene, float t);
void 		get_frame_buf(t_scene *scene);
// buffers_material_buf.c
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
int    		cl_init(t_scene *scene);

t_texture	*tex_new_bmp(char *file);
t_texture	*tex_new_surface(SDL_Surface *s);
char		*get_file(char *description);
t_color		get_color_tex(t_texture *texture, float x, float y, int index);
cl_float3		mapping_sphere(cl_float3 t, t_object *obj, int index);
cl_float3 	text_map_select(t_object *obj, cl_float3 t, int index);
cl_float3 	mapping_plane(cl_float3 t, t_object *obj);
cl_float3		mapping_cylinder(cl_float3 t, t_object *obj);
cl_float3		mapping_cone(cl_float3 t, t_object *obj);
cl_float3		normalize(cl_float3 vec);
cl_float3		mapping_triangle(cl_float3 t, t_object *obj);
t_object 	*multiple_spheres(char **description, t_scene *scene, int *snmi, int i);
void	one_argument_sphere(char **description, t_scene *scene, int *snmi);
void	one_argument_cylinder(char **description, t_scene *scene, int *snmi);
t_object 	*multiple_cylinders(char **description, t_scene *scene, int *snmi, int i);
t_object 	*multiple_cones(char **description, t_scene *scene, int *snmi, int i);
void	one_argument_cylinder(char **description, t_scene *scene, int *snmi);
void	one_argument_plane(char **description, t_scene *scene, int *snmi);
t_object 	*multiple_planes(char **description, t_scene *scene, int *snmi, int i);
void	one_argument_triangle(char **description, t_scene *scene, int *snmi, float specular);
t_object 	*multiple_triangles(char **description, int *snmi, int i, float specular);
void	one_argument_cone(char **description, t_scene *scene, int *snmi);
//texture_loading
void    load_textures(t_scene *scene);
void        intersect_ray_ellipsoid(t_scene *scene, int index);
t_object    *new_ellipsoid(cl_float3 center, cl_float3 abc, t_color color, cl_float3 rotation, float specular);
///box
void        intersect_ray_box(t_scene *scene, int index);
t_object    *new_box(cl_float3 center, cl_float3 a, cl_float3 b, t_color color, cl_float3 rotation, float specular);

void	device_objects_init(t_scene *scene);
t_basis get_default(t_basis basis);
t_basis get_basis(t_basis basis, float 	*rot);
cl_float3   clvec_rot_x(cl_float3 v, double a);
cl_float3   clvec_rot_y(cl_float3 v, double a);
cl_float3   clvec_rot_z(cl_float3 v, double a);
void			draw_ui(SDL_Renderer *renderer, t_ui *rt_ui);

#endif
