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
# include <cuda_runtime_api.h>
# include <vector_types.h>
# include <cuda.h>
# include <SDL2/SDL.h>

typedef	struct			s_scene t_scene;

enum object_type{
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
typedef struct		s_sdl
{
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	SDL_Event		event;
}					t_sdl;

typedef struct		s_ray
{
	float3		start;
	float3		dir;
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
	float			intensity;
	float3			position;
	float3			direction;
	int				type;
}					t_light;

typedef	struct		s_sphere
{
	float3			center;
	float			radius;
}					t_sphere;

typedef struct		s_plane
{
	float3		normal;
	float3		point;
	float			d;
}					t_plane;

typedef	struct		s_cylinder
{
	float3		position;
	float3		vec;
	float			radius;
}					t_cylinder;

typedef	struct		s_cone
{
	float3		position;
	float3		vec;
	float			angle;
}					t_cone;

typedef	struct		s_triangle
{
	float3		*vertex;
	float3		normal;
}					t_triangle;

typedef	struct		s_triangle_d
{
	float3		vertex[3];
	float3		normal;
}					t_triangle_d;

typedef struct		s_camera
{
	float3			position;
	float			rotation[3];
}					t_camera;

typedef	struct		s_material
{
	t_color			color;
	float			specular;
}					t_material;

typedef	struct		s_object3d
{
	void			*data;
	int				data_size;
	const char		*tag;
	float			rotation[3];
	t_color			color;
	int 			type;
	float			specular;
	void			(*get_normal)(struct s_scene *, int, int);
	void			(*intersect)(t_scene *, int);
	void			(*clear_obj)(struct s_object3d *);
}					t_object;

typedef struct 		s_ellipsoid
{
	float			a;
	float			b;
	float			c;
	float3			center;
}					t_ellipsoid;

typedef struct 		s_hyperboloid
{
	float			a;
	float			b;
	float			c;
	float3			center;
}					t_hyperboloid;

typedef struct 		s_paraboloid
{
	float 			p;
	float 			q;
	float3 			center;
}					t_paraboloid;

typedef	struct		s_object3d_d
{
	int 			type;
	t_sphere		sphere;
	t_plane			plane;
	t_triangle_d	triangle;
	t_cone			cone;
	t_cylinder		cylinder;
	t_ellipsoid		ellipsoid;
	t_paraboloid	paraboloid;
	t_hyperboloid	hyperboloid;
	t_color			color;
	float			specular;
}					t_object_d;

typedef struct 			s_scene_device
{
	t_color				*frame_buf;
	float3				*ray_buf;
	float3				*intersection_buf;
	float3				*viewport;
	int					*index_buf;
	float				*depth_buf;
	float3				*normal_buf;
	t_material			*material_buf;
	float3				*camera;
	t_light				*light;
	int					light_nmb;
	t_object_d			*obj;
}						t_scene_device;

struct		s_scene
{
	t_scene_device	*device_data;
	t_object		**objs;	
	int				obj_nmb;
	float3			*normal_buf;
	t_material		*material_buf;
	float3			*intersection_buf;
	float3			*ray_buf;
	t_light			**light;
	int				light_nmb;
	t_camera		camera;
	float3			*viewport;
	int				*index_buf;
	float			*depth_buf;
	t_color			*frame_buf;
	int				mode;
	void			(*init[4])(struct s_scene *);
	void			(*draw[4])(t_sdl *, struct s_scene *);
};

#endif
