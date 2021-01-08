/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:02 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/11 12:15:23 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

extern "C"{
	#include "rt_host.h"
	}
	#include "rt_device.cuh"

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

int	main(int args, char **argv)
{
 	t_sdl		sdl;
	t_scene		*scene;
	int			k;

	k = 1;
	scene = (t_scene *)protected_malloc(sizeof(t_scene), 1);
	args_check(scene, argv, args);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WID, HEI, 0, &sdl.win, &sdl.renderer);
	SDL_RenderClear(sdl.renderer);
 	scene->objs[1] = (t_object *)malloc(sizeof(t_object));
	t_ellipsoid *el;
	el = (t_ellipsoid *)malloc(sizeof(t_ellipsoid));
	el->a = 5;
	el->b = 5;
	el->c = 1;
	el->center = get_point(0,0,0);
	scene->objs[1]->data = (void *)el;
	scene->objs[1]->color.red = 0;
	scene->objs[1]->color.blue = 255;
	scene->objs[1]->color.green = 0;
	scene->objs[1]->color.alpha = 255;
	scene->objs[1]->type = ELLIPSOID;
	scene->objs[1]->specular = 100;
	scene->objs[1]->intersect = &intersect_ray_ellipsoid;
	//Создание гиперболоида
	/* t_hyperboloid *p;
	p = (t_hyperboloid *)malloc(sizeof(t_hyperboloid));
	p->a = 1;
	p->b = 1;
	p->c = 3;
	p->center = get_point(0,0,0);
	scene->objs[1]->data = (void *)p;
	scene->objs[1]->color.red = 0;
	scene->objs[1]->color.blue = 255;
	scene->objs[1]->color.green = 0;
	scene->objs[1]->color.alpha = 255;
	scene->objs[1]->type = HYPERBOLOID;
	scene->objs[1]->specular = 100;
	scene->objs[1]->intersect = &intersect_ray_hyperboloid; */
/* 	t_paraboloid *p;
	p = (t_paraboloid *)malloc(sizeof(t_paraboloid));
	p->q = 1;
	p->p = 5;
	p->center = get_point(0,0,0);
	scene->objs[1]->data = (void *)p;
	scene->objs[1]->color.red = 0;
	scene->objs[1]->color.blue = 255;
	scene->objs[1]->color.green = 0;
	scene->objs[1]->color.alpha = 255;
	scene->objs[1]->type = PARABOLOID;
	scene->objs[1]->specular = 100;
	scene->objs[1]->intersect = &intersect_ray_paraboloid; */
	init_scene(scene);
	scene->draw[scene->mode](&sdl, scene);
	while (k)
		k = keyboard(&sdl, scene);
	clean_scene(scene);
	SDL_DestroyRenderer(sdl.renderer);
	SDL_DestroyWindow(sdl.win);
	SDL_Quit();
	return (0);
}
