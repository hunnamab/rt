/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:58 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/18 20:32:25 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include <stdint.h>

typedef	struct			s_scene t_scene;

enum object_type {
	SPHERE,
	CONE,
	TRIANGLE,
	CYLINDER,
	PLANE,
	ELLIPSOID,
	HYPERBOLOID,
	PARABOLOID
};

enum light_type{
	POINT,
	AMBIENT,
	DIRECTIONAL
};

typedef struct 			s_scene_device
{
	cl_mem				ray_buf;
	cl_mem				intersection_buf;
	cl_mem				viewport;
	cl_mem				index_buf;
	cl_mem				depth_buf;
	cl_mem				normal_buf;
	cl_mem				material_buf;
	cl_mem				frame_buf;
	cl_mem				obj;
	cl_mem				light;
}						t_scene_device;

typedef	struct 			s_cl_data
{
	cl_device_id		device_id; // compute device id
	cl_context			context; // compute context
	cl_command_queue	commands; // compute command queue
	cl_program			*programs;
	cl_kernel 			*kernels; // compute kernel
	t_scene_device		scene;
}						t_cl_data;

typedef struct		s_sdl
{
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	SDL_Event		event;
}					t_sdl;

typedef struct		s_ray
{
	cl_float3		start;
	cl_float3		dir;
}					t_ray;

typedef struct 		s_color
{
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
	unsigned char	alpha;
}					t_color;

typedef	struct		s_light
{
	cl_float3		position;
	cl_float3		direction;
	float			intensity;
	int				type;
}					t_light;

typedef struct 		s_sphere
{
	cl_float3		center;
	cl_float		radius;
}					t_sphere;

typedef struct 		s_plane
{
	cl_float3		normal;
	cl_float3		point;
	float			d;
}					t_plane;

typedef struct 		s_cylinder
{
	cl_float3		position;
	cl_float3		vec;
	cl_float		radius;
}					t_cylinder;

typedef struct 		s_cone
{
	cl_float3		position;
	cl_float3		vec;
	float			angle;
}					t_cone;

typedef struct 		s_triangle
{
	cl_float3		vertex[3];
	cl_float3		normal;
}					t_triangle;

typedef struct		s_camera
{
	cl_float3		position;
	float			rotation[3];
}					t_camera;

typedef	struct		s_material
{
	t_color			color;
	float			specular;
}					t_material;

typedef	struct 		s_ellipsoid
{
	cl_float3 		abc;
	cl_float3		center;
}					t_ellipsoid;

typedef struct 		s_texture_d
{
	int				bytes_per_pixel;
	int				l_size;
	int				size;
	int				width;
	int				height;
	int				endian;
}					t_texture_d;

typedef	struct		s_texture
{
	SDL_Surface		*img;
	char			*pixels;
	int				bytes_per_pixel;
	int				l_size;
	int				size;
	int				width;
	int				height;
	int				endian;
	SDL_PixelFormat	*format;
}					t_texture;

typedef	struct		 __attribute__((aligned(256))) s_cutting_surface
{
	cl_int			type;
	t_sphere		sphere;
	t_plane			plane;
	t_triangle		triangle;
	t_cone			cone;
	t_cylinder		cylinder;
}					t_cutting_surface;

typedef	struct		s_object3d
{
	void			*data;
	const char		*tag;
	float			rotation[3];
	t_color			color;
	float			specular;
	t_texture		*text;
	int 			type;
	int				texture_id;
	t_cutting_surface *cutting_surfaces;
	int					cs_nmb; /*количество секущих поверхностей*/
	void			(*get_normal)(struct s_scene *, int, int);
	void			(*intersect)(t_scene *, int);
	void			(*clear_obj)(struct s_object3d *);
}					t_object;

typedef	union			primitive
{
	t_cylinder			cylinder;
	t_cone				cone;
	t_sphere			sphere;
	t_plane				plane;
	t_triangle			triangle;
	t_ellipsoid			ellipsoid;
}						t_primitive;

typedef struct 			 s_object3d_d
{	
	t_primitive			primitive;
	cl_float3			rotation;
	t_color				color;
	cl_float			specular;
	cl_float			roughness;
	cl_float			refraction;
	cl_int				color_disrupt;
	cl_int 				type;
	cl_int				texture_id;
}						t_object_d;

struct		s_scene
{
	t_cl_data		cl_data;
	t_object		**objs;	
	int				obj_nmb;
	cl_float3		*normal_buf;
	t_material		*material_buf;
	cl_float3		*intersection_buf;
	cl_float3		*ray_buf;
	t_light			*light;
	int				light_nmb;
	t_camera		camera;
	cl_float3		*viewport;
	int				*index_buf;
	float			*depth_buf;
	t_texture		**texts;
	t_color			*pixels;
	int				mode; // 0 - default, 1 - normal, 2 - depth, 3 - flat_light
	void			(*init[4])(struct s_scene *);
	void			(*draw[4])(t_sdl *, struct s_scene *);
};

#endif
