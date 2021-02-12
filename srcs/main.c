/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 00:17:47 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/12 22:06:01 by ldeirdre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	check_mode(int as, char **av, t_scene *scene)
{
	if (as == 2 || (as == 3 && (ft_strequ(av[2], "default"))))
		scene->mode = 0;
	else if (as == 3 && (ft_strequ(av[2], "normal")))
		scene->mode = 1;
	else if (as == 3 && (ft_strequ(av[2], "depth")))
		scene->mode = 2;
	else if (as == 3 && (ft_strequ(av[2], "raycast")))
		scene->mode = 3;
	else
	{
		output_error(7);
		ft_memdel((void **)scene);
		exit(0);
	}
}

void	args_check(t_scene *scene, char **argv, int args)
{
	int			fd;

	fd = 0;
	if (args < 2 || args > 3)
	{
		output_description();
		free(scene);
		exit(0);
	}
	else
	{
		check_mode(args, argv, scene);
		fd = open(argv[1], O_RDONLY);
		read_scene(fd, scene); // scene_reader.c
		close(fd);
	}
}

int		main(int args, char **argv)
{
 	t_sdl		sdl;
	t_scene		*scene;
	int			k;

	k = 1;
	scene = (t_scene *)protected_malloc(sizeof(t_scene), 1);
	//cl_init(scene->cl_data);
	args_check(scene, argv, args);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WID + 350, HEI, 0, &sdl.win, &sdl.renderer);
	SDL_RenderClear(sdl.renderer);
	scene->filter_type = DEFAULT;
/* 	scene->obj_nmb += 1;
	scene->objs[scene->obj_nmb - 1] = new_hyperboloid(get_point(0,0,0), 1, 1, 1,set_color(0,0,255,255)); */
	//scene->obj_nmb += 1;
	/* cl_float3 *buf = malloc(sizeof(cl_float3) * 2);
	buf[0] = get_point(0,0,0);
	buf[1] = get_point(10,10,10);
	scene->objs[scene->obj_nmb - 1] = new_box(buf, set_color(0,0,255,255), 100); */
	//scene->objs[scene->obj_nmb - 1] = new_paraboloid(get_point(0,0,5), 50, set_color(255,0,0,255), get_point(0,0,0), 100);
	/*инициализация секущих поверхностей
	в будущем должна производиться из фаила*/
/* 	scene->objs[0]->cs_nmb = 1;
  	scene->objs[0]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * scene->objs[0]->cs_nmb); */
/* 	scene->objs[0]->cutting_surfaces->is_negative = 0;
	scene->objs[0]->cutting_surfaces[0].primitive.plane.normal = get_point(0,1,0);
	scene->objs[0]->cutting_surfaces[0].primitive.plane.point = get_point(0,0,0);
	scene->objs[0]->cutting_surfaces[0].primitive.plane.d = -scene->objs[0]->cutting_surfaces[0].primitive.plane.normal.x * scene->objs[0]->cutting_surfaces[0].primitive.plane.point.x - scene->objs[0]->cutting_surfaces[0].primitive.plane.\
	normal.y * scene->objs[0]->cutting_surfaces[0].primitive.plane.point.y - scene->objs[0]->cutting_surfaces[0].primitive.plane.normal.z * scene->objs[0]->cutting_surfaces[0].primitive.plane.point.z;
	scene->objs[0]->cutting_surfaces[0].type = PLANE; */
/* 	scene->objs[0]->cutting_surfaces[0].is_negative = 1;
	scene->objs[0]->cutting_surfaces[0].primitive.sphere.center = get_point(0,0,0);
	scene->objs[0]->cutting_surfaces[0].primitive.sphere.radius = 10;
	scene->objs[0]->cutting_surfaces[0].type = SPHERE; */
/* 	scene->objs[0]->cutting_surfaces[1].primitive.plane.normal = get_point(0,1,0);
	scene->objs[0]->cutting_surfaces[1].primitive.plane.point = get_point(0,0,0);
	scene->objs[0]->cutting_surfaces[1].primitive.plane.d = -scene->objs[0]->cutting_surfaces[1].primitive.plane.normal.x * scene->objs[0]->cutting_surfaces[1].primitive.plane.point.x - scene->objs[0]->cutting_surfaces[1].primitive.plane.\
	normal.y * scene->objs[0]->cutting_surfaces[1].primitive.plane.point.y - scene->objs[0]->cutting_surfaces[1].primitive.plane.normal.z * scene->objs[0]->cutting_surfaces[1].primitive.plane.point.z;
	scene->objs[0]->cutting_surfaces[1].type = PLANE; */
	/***********************************/
	cl_init(scene); // cl_init.c, инициализируем кернели для дальнейшей работы
	init_scene(scene);
	init_ui(&sdl, scene);
	filters_init(&scene->filter_data);
	scene->draw[scene->mode](&sdl, scene);
	init_music();
	while (k)
		k = keyboard(&sdl, scene);
	clean_scene(scene);
	SDL_DestroyRenderer(sdl.renderer);
	SDL_DestroyWindow(sdl.win);
	SDL_Quit();
	return (0);
}
