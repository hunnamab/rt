/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:58 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/10 13:36:22 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H
# include <stdint.h>

typedef	struct		s_scene t_scene;

typedef struct		s_sdl
{
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	SDL_Event		event;
}					t_sdl;

typedef struct		s_point
{
	double			x;
	double			y;
	double			z;
}					t_point;

typedef struct		s_ray
{
	t_point			start;
	t_point			dir;
}					t_ray;

typedef	struct		s_color
{
	uint8_t			red;
	uint8_t			green;
	uint8_t			blue;
	uint8_t			alpha;
}					t_color;

typedef	struct		s_light
{
	double			intensity;
	t_point			position;
	t_point			direction;
	const char		*type;
}					t_light;

typedef	struct		s_sphere
{
	t_point			center;
	double			radius;
}					t_sphere;

typedef struct		s_plane
{
	t_point			normal;
	t_point			point;
	double			d;
}					t_plane;

typedef	struct		s_cylinder
{
	t_point			position;
	t_point			vec;
	double			radius;
}					t_cylinder;

typedef	struct		s_cone
{
	t_point			position;
	t_point			vec;
	double			angle;
}					t_cone;

typedef	struct		s_triangle
{
	t_point			*vertex;
	t_point			normal;
}					t_triangle;

typedef struct		s_camera
{
	t_point			position;
	double			rotation[3];
}					t_camera;

typedef	struct		s_material
{
	t_color			color;
	double			specular;
}					t_material;

typedef	struct		s_object3d
{
	void			*data;
	const char		*tag;
	double			rotation[3];
	t_color			color;
	double			specular;
	void			(*get_normal)(struct s_scene *, int, int);
	double			(*intersect)(t_ray *, struct s_object3d *);
	void			(*clear_obj)(struct s_object3d *);
}					t_object;

struct		s_scene
{
	t_object		**objs;
	int				obj_nmb;
	t_point			*normal_buf;
	t_material		*material_buf;
	t_point			*intersection_buf;
	t_ray			*ray_buf;
	t_light			**light;
	int				light_nmb;
	t_camera		camera;
	t_point			*viewport;
	int				*index_buf;
	double			*depth_buf;
	int				mode; // 0 - default, 1 - normal, 2 - depth, 3 - flat_light
	void			(*init[4])(struct s_scene *);
	void			(*draw[4])(t_sdl *, struct s_scene *);
};

#endif
