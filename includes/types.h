/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:33:03 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/19 07:23:43 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# define FILTERS_NUM 5
# include <stdint.h>

typedef	struct s_scene	t_scene;

enum					e_object_type {
	SPHERE,
	CONE,
	TRIANGLE,
	CYLINDER,
	PLANE,
	ELLIPSOID,
	HYPERBOLOID,
	PARABOLOID,
	BOX,
	TORUS
};

enum					e_light_type {
	POINT,
	AMBIENT,
	DIRECTIONAL
};

typedef struct			s_scene_device
{
	cl_mem				ray_buf;
	cl_mem				intersection_buf;
	cl_mem				copy_intersec_buf;
	cl_mem				viewport;
	cl_mem				index_buf;
	cl_mem				orig_index_buf;
	cl_mem				exception_buf;
	cl_mem				depth_buf;
	cl_mem				normal_buf;
	cl_mem				copy_normal_buf;
	cl_mem				material_buf;
	cl_mem				prev_material_buf;
	cl_mem				frame_buf;
	cl_mem				frame_buf_refl;
	cl_mem				frame_buf_refr;
	cl_mem				obj;
	cl_mem				light;
	cl_mem				textures;
}						t_scene_device;

typedef	struct			s_cl_data
{
	cl_device_id		device_id;
	cl_context			context;
	cl_command_queue	commands;
	cl_program			*programs;
	cl_kernel			*kernels;
	t_scene_device		scene;
}						t_cl_data;

typedef struct			s_sdl
{
	SDL_Window			*win;
	SDL_Renderer		*renderer;
	SDL_Event			event;
}						t_sdl;

typedef struct			s_ray
{
	cl_float3			start;
	cl_float3			dir;
}						t_ray;

typedef struct			s_color
{
	unsigned char		red;
	unsigned char		green;
	unsigned char		blue;
	unsigned char		alpha;
}						t_color;

typedef	struct			s_light
{
	cl_float3			position;
	cl_float3			direction;
	float				intensity;
	int					type;
}						t_light;

typedef struct			s_sphere
{
	cl_float3			center;
	cl_float			radius;
}						t_sphere;

typedef struct			s_plane
{
	cl_float3			normal;
	cl_float3			point;
	float				d;
}						t_plane;

typedef struct			s_cylinder
{
	cl_float3			position;
	cl_float3			vec;
	cl_float			radius;
}						t_cylinder;

typedef struct			s_cone
{
	cl_float3			position;
	cl_float3			vec;
	float				angle;
}						t_cone;

typedef struct			s_box
{
	cl_float3			a;
	cl_float3			b;
	int					face_hit;
}						t_box;

typedef struct			s_triangle
{
	cl_float3			vertex[3];
	cl_float3			normal;
}						t_triangle;

typedef struct			s_camera
{
	cl_float3			position;
	float				rotation[3];
}						t_camera;

typedef	struct			s_material
{
	t_color				color;
	float				specular;
	float				reflection;
	float				refraction;
	float				transparency;
	float				kr;
}						t_material;

typedef	struct			s_ellipsoid
{
	cl_float3			center;
	float				a;
	float				b;
	float				c;
}						t_ellipsoid;

typedef struct			s_paraboloid
{
	cl_float3			center;
	cl_float3			vec;
	float				k;
}						t_paraboloid;

typedef struct			s_torus
{
	cl_float3			center;
	cl_float3			vec;
	float				radius1;
	float				radius2;
}						t_torus;

typedef struct			s_hyperboloid
{
	cl_float3			center;
	float				a;
	float				b;
	float				c;
}						t_hyperboloid;

typedef struct			s_texture_d
{
	int					bytes_per_pixel;
	int					l_size;
	int					size;
	int					width;
	int					height;
	int					endian;
}						t_texture_d;

typedef	struct			s_texture
{
	SDL_Surface			*img;
	char				*pixels;
	int					bytes_per_pixel;
	int					l_size;
	int					size;
	int					width;
	int					height;
	int					endian;
	SDL_PixelFormat		*format;
}						t_texture;

typedef struct			s_basis
{
	cl_float3			u;
	cl_float3			v;
	cl_float3			w;
}						t_basis;

typedef struct			s_butt
{
	float				x;
	float				y;
	SDL_Rect			rect;
	SDL_Texture			*text;
}						t_butt;

typedef	struct			s_ui
{
	t_butt				save_png;
	t_butt				ambiance;
	t_butt				am_plus;
	t_butt				am_minus;
	t_butt				modes;
	t_butt				normal;
	t_butt				deepth;
	t_butt				raycast;
	t_butt				def;
	t_butt				back;
	t_butt				left;
	t_butt				right;
	t_butt				filters;
	t_butt				sepia;
	t_butt				gauss;
	t_butt				grey;
	t_butt				fault;
	t_butt				negative;
	t_butt				magic;
	t_butt				sound;
	int					filt;
	int					i;
}						t_ui;

typedef	union			u_primitive
{
	t_cylinder			cylinder;
	t_cone				cone;
	t_sphere			sphere;
	t_plane				plane;
	t_triangle			triangle;
	t_ellipsoid			ellipsoid;
	t_hyperboloid		hyperboloid;
	t_box				box;
	t_paraboloid		paraboloid;
	t_torus				torus;
}						t_primitive;

typedef	struct			s_cutting_surface
{
	cl_float3			param1;
	cl_float3			param2;
	cl_int				type;
	cl_int				object;
	cl_int				is_local;
	cl_int				is_negative;
	cl_float			param3;
}						t_cutting_surface;

typedef	struct			s_object3d
{
	void				*data;
	const char			*tag;
	float				rotation[3];
	cl_float3			position;
	t_color				color;
	float				specular;
	float				reflection;
	float				refraction;
	float				transparency;
	t_texture			*text;
	t_texture			*normal_text;
	t_basis				basis;
	int					type;
	int					t_scale;
	int					texture_id;
	int					surface_id;
	int					normal_map_id;
	t_cutting_surface	*cutting_surfaces;
	int					cs_nmb;
	void				(*get_normal)(struct s_scene *, int, int);
	void				(*intersect)(t_scene *, int, int);
	void				(*clear_obj)(struct s_object3d *);
}						t_object;

typedef struct			s_object3d_d
{
	t_primitive			primitive;
	t_basis				basis;
	cl_float3			rotation;
	t_color				color;
	cl_float			specular;
	cl_float			roughness;
	cl_float			refraction;
	cl_float			reflection;
	cl_float			transparency;
	cl_int				color_disrupt;
	cl_int				type;
	cl_int				t_scale;
	cl_int				texture_id;
	cl_int				texture_size;
	cl_int				texture_width;
	cl_int				texture_height;
	cl_int				l_size;
	cl_int				normal_map_id;
	cl_int				texture_size_nm;
	cl_int				texture_width_nm;
	cl_int				texture_height_nm;
	cl_int				l_size_nm;
}						t_object_d;
typedef struct			s_filter_data
{
	cl_program			*programs;
	cl_kernel			*kernels;
	cl_context			context;
	cl_command_queue	commands;
	cl_device_id		device_id;
	cl_mem				pixels;
}						t_filter_data;

struct					s_scene
{
	t_cl_data			cl_data;
	t_object			**objs;
	int					obj_nmb;
	t_ui				*rt_ui;
	cl_float3			*normal_buf;
	t_material			*material_buf;
	cl_float3			*intersection_buf;
	cl_float3			*ray_buf;
	t_filter_data		filter_data;
	t_light				*light;
	int					light_nmb;
	t_camera			camera;
	cl_float3			*viewport;
	int					*index_buf;
	float				*depth_buf;
	t_texture			**texts;
	t_color				*frame_buf;
	t_cutting_surface	*srfs;
	int					init_flag;
	int					srf_nmb;
	int					box_nmb;
	int					scrshot_nmb;
	int					filter_type;
	int					mode;
	int					max_bounces;
	int					bounce_cnt;
	int					has_refraction;
	int					m_flag;
	void				(*init[4])(struct s_scene *);
	void				(*draw[4])(t_sdl *, struct s_scene *);
	void				(*filter[10])(t_filter_data *);
};

#endif
