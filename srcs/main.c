/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 00:17:47 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/21 20:51:33 by npetrell         ###   ########.fr       */
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
		read_scene(fd, scene);
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
	args_check(scene, argv, args);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(WID + 350, HEI, 0, &sdl.win, &sdl.renderer);
	SDL_RenderClear(sdl.renderer);
	scene->filter_type = DEFAULT;
	cl_init(scene);
	init_scene(scene);
	local_cutting(scene);
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
