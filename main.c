/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:02 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/18 20:08:32 by pmetron          ###   ########.fr       */
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

void			init_music(void)
{
	Mix_Music	*mus;
	int			result;
	int			flags;

	result = 0;
	flags = MIX_INIT_OGG;
	if (flags != (result = Mix_Init(flags)))
		exit(1);
	Mix_OpenAudio(22050, AUDIO_S16SYS, 4, 640);
	mus = Mix_LoadMUS("lights.wav");
	Mix_PlayMusic(mus, -1);
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
	SDL_CreateWindowAndRenderer(WID, HEI, 0, &sdl.win, &sdl.renderer);
	SDL_RenderClear(sdl.renderer);
	/*инициализация секущих поверхностей
	в будущем должна производиться из фаила*/
	//scene->objs[0]->cs_nmb = 0;
/*  	scene->objs[0]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * scene->objs[0]->cs_nmb);
	scene->objs[0]->cutting_surfaces[0].plane.normal = get_point(1,0,0);
	scene->objs[0]->cutting_surfaces[0].plane.point = get_point(0,0,0);
	scene->objs[0]->cutting_surfaces[0].plane.d = -scene->objs[0]->cutting_surfaces[0].plane.normal.x * scene->objs[0]->cutting_surfaces[0].plane.point.x - scene->objs[0]->cutting_surfaces[0].plane.\
	normal.y * scene->objs[0]->cutting_surfaces[0].plane.point.y - scene->objs[0]->cutting_surfaces[0].plane.normal.z * scene->objs[0]->cutting_surfaces[0].plane.point.z;
	scene->objs[0]->cutting_surfaces[0].type = PLANE; */
	/* scene->objs[0]->cutting_surfaces[1].plane.normal = get_point(0,1,0);
	scene->objs[0]->cutting_surfaces[1].plane.point = get_point(0,0,0);
	scene->objs[0]->cutting_surfaces[1].plane.d = -scene->objs[0]->cutting_surfaces[1].plane.normal.x * scene->objs[0]->cutting_surfaces[1].plane.point.x - scene->objs[0]->cutting_surfaces[1].plane.\
	normal.y * scene->objs[0]->cutting_surfaces[1].plane.point.y - scene->objs[0]->cutting_surfaces[1].plane.normal.z * scene->objs[0]->cutting_surfaces[1].plane.point.z;
	scene->objs[0]->cutting_surfaces[1].type = PLANE; */
	/***********************************/
	cl_init(scene); // cl_init.c, инициализируем кернели для дальнейшей работы
	load_textures(scene);
	init_scene(scene);
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
