/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 00:17:47 by npetrell          #+#    #+#             */
/*   Updated: 2021/02/15 22:37:10 by pmetron          ###   ########.fr       */
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
	printf("light pos (%f,%f,%f) dir (%f,%f,%f)\n", scene->light[0].position.x, scene->light[0].position.y,scene->light[0].position.z,scene->light[0].direction.z,scene->light[0].direction.y,scene->light[0].direction.z);
	scene->light[0].direction.z = -1;
	scene->filter_type = DEFAULT;
	/***************BOX_CREATION***********/
/* 	cl_float3	pn[3];
	cl_float3	srfp[2];
	int			srfp2[3];
	float		size;
	cl_float3	position;
	size = 5;
	position = get_point(1,2,3);
	srfp2[0] = 0;
	srfp2[1] = 0;
	srfp2[2] = 0;
	float		parameters[6];
	parameters[0] = 100;
	parameters[1] = 0;
	parameters[2] = 0;
	parameters[3] = 0;
	parameters[4] = 10;
	parameters[5] = 0;
	scene->objs = malloc(sizeof(t_object *) * 6);
	scene->obj_nmb = 6;
	pn[0] = get_point(position.x + size / 2, position.y ,position.z);
	pn[1] = get_point(1,0,0);
	pn[2] = get_point(0,0,0);
	scene->objs[0] = new_plane(pn, parameters, set_color(0,255,0,0));
	scene->objs[0]->cs_nmb = 4;
	scene->objs[0]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position.x + size / 2, position.y + size / 2, position.z);
	scene->objs[0]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position.x + size / 2, position.y - size / 2, position.z);
	scene->objs[0]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position.x + size / 2, position.y , position.z + size / 2);
	scene->objs[0]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position.x + size / 2, position.y, position.z - size / 2);
	srfp2[2] = 0;
	scene->objs[0]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//second plane
	pn[0] = get_point(position.x - size / 2, position.y, position.z);
	pn[1] = get_point(-1,0,0);
	scene->objs[1] = new_plane(pn, parameters, set_color(0,255,0,0));
	scene->objs[1]->cs_nmb = 4;
	scene->objs[1]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position.x - size / 2, position.y + size / 2, position.z);
	scene->objs[1]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position.x - size / 2, position.y - size / 2, position.z);
	scene->objs[1]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z + size / 2);
	scene->objs[1]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z - size / 2);
	scene->objs[1]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//third plane
	pn[0] = get_point(position.x, position.y, position.z + size / 2);
	pn[1] = get_point(0,0,1);
	scene->objs[2] = new_plane(pn, parameters, set_color(0,255,0,0));
	scene->objs[2]->cs_nmb = 4;
	scene->objs[2]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position.x, position.y + size / 2, position.z);
	scene->objs[2]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position.x, position.y - size / 2, position.z);
	scene->objs[2]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position.x + size / 2, position.y, position.z);
	scene->objs[2]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z);
	scene->objs[2]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//fourth plane
	pn[0] = get_point(position.x, position.y + size / 2, position.z);
	pn[1] = get_point(0,1,0);
	scene->objs[3] = new_plane(pn, parameters, set_color(0,255,0,0));
	scene->objs[3]->cs_nmb = 4;
	scene->objs[3]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position.x + size / 2, position.y, position.z + size / 2);
	scene->objs[3]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z - size / 2);
	scene->objs[3]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position.x + size / 2, position.y, position.z);
	scene->objs[3]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z);
	scene->objs[3]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//fifth plane
	pn[0] = get_point(position.x, position.y - size / 2, position.z);
	pn[1] = get_point(0,-1,0);
	scene->objs[4] = new_plane(pn, parameters, set_color(0,255,0,0));
	scene->objs[4]->cs_nmb = 4;
	scene->objs[4]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,0,1);
	srfp[1] = get_point(position.x + size / 2, position.y, position.z + size / 2);
	scene->objs[4]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,0,-1);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z - size / 2);
	scene->objs[4]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position.x + size / 2, position.y, position.z);
	scene->objs[4]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z);
	scene->objs[4]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\",");
	//last
	pn[0] = get_point(position.x, position.y, position.z - size / 2);
	pn[1] = get_point(0,0,-1);
	scene->objs[5] = new_plane(pn, parameters, set_color(0,255,0,0));
	scene->objs[5]->cs_nmb = 4;
	scene->objs[5]->cutting_surfaces = malloc(sizeof(t_cutting_surface) * 4);
	srfp[0] = get_point(0,1,0);
	srfp[1] = get_point(position.x, position.y + size / 2, position.z);
	scene->objs[5]->cutting_surfaces[0] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(0,-1,0);
	srfp[1] = get_point(position.x, position.y - size / 2, position.z);
	scene->objs[5]->cutting_surfaces[1] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(1,0,0);
	srfp[1] = get_point(position.x + size / 2, position.y, position.z);
	scene->objs[5]->cutting_surfaces[2] = new_srf(srfp, srfp2, 0, "\"plane\",");
	srfp[0] = get_point(-1,0,0);
	srfp[1] = get_point(position.x - size / 2, position.y, position.z);
	scene->objs[5]->cutting_surfaces[3] = new_srf(srfp, srfp2, 0, "\"plane\","); */
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
