/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:34 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/22 18:23:16 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H
# ifdef __APPLE__
#  include <OpenCL/opencl.h>
#  include "../frameworks/SDL2.framework/Headers/SDL.h"
#  include "../frameworks/SDL_image/SDL_image.h"
#  include "../frameworks/SDL_mixer/SDL_mixer.h"
# else
#  include <SDL2/SDL.h>
#  include <CL/cl.h>
# endif
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <stdint.h>
# include "libft.h"
# include "../ft_printf/includes/ft_printf.h"
# include <unistd.h>
# include "../matrix_lib/matrix.h"
# include "types.h"
# include "filters.h"

# define ABS(x)		(x) > 0 ? (x) : -(x)
# define DROUND(d)	ABS(d) < 0.00001 ? 0 : (d)
# define DTR(k)		((double)k * (M_PI / 180))

# define WID		1280
# define HEI		720
# define KERNEL_NUM	20

void				read_scene(int fd, t_scene *scene);
void				split_objects(int len, t_scene *scene, char *buf);
int					count_objects(int len, char *buf);
char				**get_description(char *scene, int i);
void				get_parameters(char *name, char **description,
										t_scene *scene, int *snmi);
void				draw_scene(t_sdl *sdl, t_scene *scene);
void				draw_normal_buf(t_sdl *sdl, t_scene *scene);
void				draw_deepth_buf(t_sdl *sdl, t_scene *scene);
void				draw_raycast(t_sdl *sdl, t_scene *scene);
t_color				reflection_color(t_scene *scene, int index);
t_light				new_light(cl_float3 *pos_dir, int type, float intensity);
cl_float3			get_light_vec(t_scene *scene, int index, int j);
float				get_specular(t_scene *scene, int index, \
												int j, cl_float3 *l);
int					in_shadow(t_scene *scene, int index, cl_float3 l);
cl_float3			vector_add(const cl_float3 *v1, const cl_float3 *v2);
cl_float3			vector_scale(cl_float3 *v, float c);
cl_float3			vector_sub(cl_float3 *v1, cl_float3 *v2);
float				vector_dot(cl_float3 *v1, cl_float3 *v2);
cl_float3			vector_cross(cl_float3 *v1, cl_float3 *v2);
float				vector_length(cl_float3 *vector);
cl_float3			vector_div_by_scalar(cl_float3 *vector, float scalar);
cl_float3			vector_sub_by_scalar(cl_float3 *vector, float scalar);
void				normalize_vector(cl_float3 *v1);
cl_float3			vector_add_scalar(const cl_float3 *v1, const float scalar);
cl_float3			get_point(float x, float y, float z);
void				get_viewport(t_scene *scene);
void				copy_point(cl_float3 *dst, cl_float3 *src);
float				choose_t(float t0, float t1);
void				clear_default(t_object *obj);
void				*protected_malloc(unsigned int size, unsigned int nmb);
void				copy_color(t_color *dst, t_color *src);
t_color				set_color(uint8_t red, uint8_t green, \
										uint8_t blue, uint8_t alpha);
void				set_color_zero(t_color *color);
t_color				color_mul_by_scalar(t_color *color, float scalar);
void				intersect_ray_sphere(t_scene *scene, \
											int index, int is_refractive);
t_object			*new_sphere(cl_float3 center, float *rad_spec, \
												t_color color, float *rotation);
void				intersect_ray_triangle(t_scene *scene, \
												int index, int is_refractive);
t_object			*new_triangle(cl_float3 *vertex, \
								float specular, t_color color, float *rotation);
void				clear_triangle(t_object *obj);
/*
** plane.c
*/
void				intersect_ray_plane(t_scene *scene, \
												int index, int is_refractive);
t_object			*new_plane(cl_float3 *poi_nor, \
											float *specular, t_color color);
/*
** cylinder.c
*/
void				intersect_ray_cylinder(t_scene *scene, \
												int index, int is_refractive);
t_object			*new_cylinder(cl_float3 *pos_vec, \
												float *rad_spec, t_color color);
/*
** cone.c
*/
void				intersect_ray_cone(t_scene *scene, \
												int index, int is_refractive);
t_object			*new_cone(cl_float3 *pos_vec, \
							float *ang_spec, t_color color, float *rotation);
void				get_sphere(char **description, t_scene *scene, int *snmi);
void				get_triangle(char **description, \
								float specular, t_scene *scene, int *snmi);
void				get_plane(char **description, t_scene *scene, int *snmi);
void				get_cylinder(char **description, t_scene *scene, int *snmi);
void				get_cone(char **description, t_scene *scene, int *snmi);
/*
** light_parameters.c
*/
void				get_light(char **description, t_scene *scene, int *snmi);
t_light				many_lights(char **description, int *snmi, int i, \
																char *type);
/*
** camera_parameters.c
*/
void				get_camera(char **description, t_scene *scene);
/*
** parameters_utils.c
*/
char				*get_coordinates(char *description);
t_color				get_color(char *description);
cl_float3			get_points(char *description);
/*
** transform.c
*/
float				**get_transform_matrix(float *angle, \
										float *move, float *scale);
float				**get_rotation_matrix(float *angle);
float				*get_three_floats(float x, float y, float z);
void				transform(cl_float3 *point, float **matrix, int point_nmb);
/*
** transform_matrix.c
*/
float				**get_translate_matrix(float *move);
float				**get_scale_matrix(float *scale);
float				**rotate_x(float x);
float				**rotate_z(float z);
float				**rotate_y(float y);
/*
** buffers.c
*/
void				get_rays_arr(t_scene *scene);
void				get_buffers(t_scene *scene);
void				get_normal_buf(t_scene *scene);
void				get_intersection_buf(t_scene *scene);
void				get_closest_points(t_scene *scene, \
											float t, int is_refractive);
void				get_frame_buf(t_scene *scene, int is_refractive);
/*
** buffers_material_buf.c
*/
void				get_material_buf(t_scene *scene, int is_refractive);
/*
** normal.c
*/
void				get_sphere_normal(t_scene *scene, int index, int obj_num);
void				get_plane_normal(t_scene *scene, int index, int obj_num);
void				get_triangle_normal(t_scene *scene, int index, int obj_num);
void				get_cone_normal(t_scene *scene, int index, int obj_num);
void				get_cylinder_normal(t_scene *scene, int index, int obj_num);
/*
** scene.c
*/
void				init_scene(t_scene *scene);
void				init_raycast(t_scene *scene);
void				init_default(t_scene *scene);
void				init_deepth(t_scene *scene);
void				refresh_scene(t_scene *scene);
/*
** keyboard.c
*/
void				camera_left(t_scene *scene);
int					keyboard(t_sdl *sdl, t_scene *scene);
void				camera_up(t_scene *scene);
void				camera_right(t_scene *scene);
void				camera_down(t_scene *scene);
/*
** scene_clear.c
*/
void				clean_scene(t_scene *scene);
/*
** errors_management.c
*/
void				output_description(void);
void				output_error(int tag);
int					cl_init(t_scene *scene);
t_texture			*tex_new_bmp(char *file);
t_texture			*tex_new_surface(SDL_Surface *s);
char				*get_file(char *description);
t_color				get_color_tex(t_texture *texture, \
												float x, float y, int index);
cl_float3			mapping_sphere(cl_float3 t, t_object *obj, int index);
cl_float3			text_map_select(t_object *obj, cl_float3 t, int index);
cl_float3			mapping_plane(cl_float3 t, t_object *obj);
cl_float3			mapping_cylinder(cl_float3 t, t_object *obj);
cl_float3			mapping_cone(cl_float3 t, t_object *obj);
cl_float3			normalize(cl_float3 vec);
cl_float3			mapping_triangle(cl_float3 t, t_object *obj);
t_object			*multiple_spheres(char **description, int i);
void				one_argument_sphere(char **description, \
										t_scene *scene, int *snmi);
void				one_argument_cylinder(char **description, \
										t_scene *scene, int *snmi);
t_object			*multiple_cylinders(char **description, int i);
t_object			*multiple_cones(char **description, int i);
void				one_argument_cylinder(char **description, \
											t_scene *scene, int *snmi);
void				one_argument_plane(char **description, \
											t_scene *scene, int *snmi);
t_object			*multiple_planes(char **description, int i);
void				one_argument_triangle(char **description, \
							t_scene *scene, int *snmi, float specular);
t_object			*multiple_triangles(char **description, \
												int i, float specular);
void				one_argument_cone(char **description, \
											t_scene *scene, int *snmi);
/*
**	texture_loading
*/
void				load_textures(t_scene *scene);
void				intersect_ray_ellipsoid(t_scene *scene, \
										int index, int is_refractive);
void				intersect_ray_box(t_scene *scene, int index, \
													int is_refractive);
void				new_box(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene);
void				test_box(t_scene *scene);
void				device_objects_init(t_scene *scene);
t_basis				get_default(t_basis basis);
t_basis				get_basis(t_basis basis, float *rot);
cl_float3			clvec_rot_x(cl_float3 v, double a);
cl_float3			clvec_rot_y(cl_float3 v, double a);
cl_float3			clvec_rot_z(cl_float3 v, double a);
void				draw_ui(SDL_Renderer *renderer, t_ui *rt_ui);
void				intersect_ray_paraboloid(t_scene *scene, \
									int index, int is_refractive);
t_object			*new_paraboloid(cl_float3 *cen_buf, \
						t_color color, float *specular, int surface_id);
void				get_ellipsoid(char **description, \
											t_scene *scene, int *snmi);
t_object			*new_ellipsoid(cl_float3 *buf, t_color color, \
											float *specular, int surface_id);
t_object			*new_torus(cl_float3 *cen_buf, t_color color, \
											float *specular, int surface_id);
void				intersect_ray_torus(t_scene *scene, int index, \
														int is_refractive);
void				get_box(char **description, t_scene *scene, int *snmi);
t_object			*new_hyperboloid(cl_float3 *cen_buf, float *specular, \
												t_color color, int surface_id);
void				intersect_ray_hyperboloid(t_scene *scene, int index, int \
																is_refractive);
void				get_torus(char **description, t_scene *scene, int *snmi);
void				get_paraboloid(char **description, t_scene *scene, \
																int *snmi);
char				*get_light_type(char *description);
void				get_hyperboloid(char **description, t_scene *scene, \
																int *snmi);
int					choose_type(char *type);
t_cutting_surface	new_srf(cl_float3 *param, int *obj_neg, cl_float param3, \
																char *type);
void				one_srf(char **description, t_scene *scene, int *snmi);
t_cutting_surface	many_srfs(char **description, int i);
void				get_surface(char **description, t_scene *scene, int *snmi);
void				init_music();
void				draw_ui(SDL_Renderer *renderer, t_ui *rt_ui);
void				init_ui(t_sdl *sdl, t_scene *scene);
void				click(t_sdl *sdl, t_scene *scene);
void				screen_png(t_scene *scene);
void				camera_left(t_scene *scene);
void				camera_right(t_scene *scene);
void				camera_up(t_scene *scene);
void				camera_down(t_scene *scene);
void				camera_forward(t_scene *scene);
void				camera_backward(t_scene *scene);
void				camera_move_right(t_scene *scene);
void				camera_move_left(t_scene *scene);
void				camera_move_up(t_scene *scene);
void				camera_move_down(t_scene *scene);
void				local_cutting(t_scene *scene);
t_texture			*calc_perlin();
int					check_rect(SDL_Rect rect, SDL_Event e);
void				am_plus(t_scene *scene);
void				am_minus(t_scene *scene);
void				redraw_mode(t_scene *scene, int mode);
void				music_control(t_scene *scene);

void				get_refraction_ray(t_scene *scene);
void				get_reflection_ray(t_scene *scene);
void				draw_scene(t_sdl *sdl, t_scene *scene);
void				draw_normal_buf(t_sdl *sdl, t_scene *scene);
void				draw_deepth_buf(t_sdl *sdl, t_scene *scene);
void				get_fresnel_coeff(t_scene *scene);
void				get_objects(char *buf, t_scene *scene, int len);
void				scene_objects(int *snmi, t_scene *scene, char *buf);
void				split_box(int *scij, t_scene *scene, char *buf);
void				strequal(char *obn, int *scij, t_scene *scene, char *buf);
void				split_surface(int *scij, t_scene *scene, char *buf);
void				split_light(int *scij, t_scene *scene, char *buf);
float				dot(cl_float2 vector1, cl_float2 vector2);
int					*shuffle(int perlin_arr[512]);
int					*make_permutation(int perlin_arr[512]);
float				fade(float t);
float				lerp(float t, float a1, float a2);
cl_float2			get_constant_vector(float x);
float				noise2d(float x, float y);
void				new_directional_light(char **description,
						t_scene *scene, int *snmi);
int					add_count(char *type, int i);
void				init_srfp2(int (*srfp2)[3]);
void				first_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene);
void				second_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene);
void				third_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene);
void				fourth_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene);
void				fifth_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene);
void				sixth_plane(cl_float3 *position, t_color color,
							float *parameters, t_scene *scene);
void				one_argument_box(char **description, \
									t_scene *scene, int *snmi);
void				multiple_boxes(char **description, int i, \
									t_scene *scene, int *snmi);
void				init_objects(t_object *new_object, cl_float3 *pos_vec, \
											float *ang_spec, t_color color);
void				init_ell(t_object *new_object, cl_float3 *buf, \
					t_color color, int surface_id);
void				hyperboloid_init(t_object *new_object, cl_float3 *cen_buf,
								t_color color, int surface_id);
void				paraboloid_init(t_object *new_object, cl_float3 *cen_buf, \
							t_color color, int surface_id);
t_basis				get_plane_default(t_basis basis, cl_float3 normal);
void				plane_init(t_object *new_object, cl_float3 *poi_nor, \
									float *specular, t_color color);
void				set_arg_plane(t_scene *scene, int index);
void				set_arg2_plane(t_scene *scene, int index, \
												int is_refractive, cl_mem cs);
void				set_arg_sphere(t_scene *scene, int index, cl_mem cs);
void				set_arg2_sphere(t_scene *scene, int index, \
													int is_refractive);
void				set_arg_torus(t_scene *scene, int index, \
						cl_mem cs);
void				set_arg2_torus(t_scene *scene, int index, \
						int is_refractive);
t_basis				get_basis_tr(t_basis basis, cl_float3 *vert);
cl_float3			cross(cl_float3 u, cl_float3 v);
cl_float3			normalize_tr(cl_float3 vec);
void				init_norme_triangle(float *r, cl_float3 *v, \
											float *rt, cl_float3 *vt);

void				copy_memory_objects(t_scene *scene, t_object_d *buf, int i);
void				init_buf(t_scene *scene, t_object_d *buf, int i);
void				fd1(t_scene *scene);
void				fd2(t_scene *scene);
void				fd3(t_scene *scene);
void				fd4(t_scene *scene);
void				fd5(t_scene *scene);
void				fd6(t_scene *scene);
void				fd7(t_scene *scene);
void				fd8(t_scene *scene);
void				fd9(t_scene *scene);
void				fd10(t_scene *scene);
void				fd11(t_scene *scene);
void				fd12(t_scene *scene);
void				fd13(t_scene *scene);
void				fd14(t_scene *scene);
void				fd15(t_scene *scene);
void				fd16(t_scene *scene);
void				fd17(t_scene *scene);
void				fd18(t_scene *scene);
void				fd19(t_scene *scene);
void				create_buf_gpu(t_scene *scene);
void				magic_filter(t_filter_data *data);
void				sharpen_filter(t_filter_data *data);
void				gauss_filter(t_filter_data *data);
void				negative(t_filter_data *data);
void				gray_scale(t_filter_data *data);
void				sepia_filter(t_filter_data *data);
void				fd_sepia(t_filter_data *data);
void				fd_negative(t_filter_data *data);
void				fd_gray_scale(t_filter_data *data);
void				fd_sharpen(t_filter_data *data);
void				fd_magic(t_filter_data *data);
void				fd_gauss(t_filter_data *data);
void				get_texture_buf(t_scene *scene, cl_int4 *index_buf, \
					unsigned char *texture_buf, long long int *ijiss);

#endif
