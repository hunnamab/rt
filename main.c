/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:02 by hunnamab          #+#    #+#             */
/*   Updated: 2021/01/30 22:08:26 by npetrell         ###   ########.fr       */
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
t_butt	init_save_butt(SDL_Renderer *renderer, SDL_Rect rect, char *text)
{
	t_butt save;
	SDL_Surface *srf;

	save.rect = rect;
	save.x = (float)(rect.x + (float)rect.w / 2) / WID;
	save.y = (float)(rect.y + (float)rect.h / 2) / HEI;
	srf = IMG_Load(text);
	save.text = SDL_CreateTextureFromSurface(renderer, srf);
	SDL_FreeSurface(srf);
	return (save);
}

void			draw_ui(SDL_Renderer *renderer, t_ui *rt_ui)
{
	SDL_RenderCopy(renderer, rt_ui->save_png.text, NULL, &rt_ui->save_png.rect);
	SDL_RenderCopy(renderer, rt_ui->ambiance.text, NULL, &rt_ui->ambiance.rect);
	SDL_RenderCopy(renderer, rt_ui->am_plus.text, NULL, &rt_ui->am_plus.rect);
	SDL_RenderCopy(renderer, rt_ui->am_minus.text, NULL, &rt_ui->am_minus.rect);
	SDL_RenderCopy(renderer, rt_ui->modes.text, NULL, &rt_ui->modes.rect);
	SDL_RenderCopy(renderer, rt_ui->normal.text, NULL, &rt_ui->normal.rect);
	SDL_RenderCopy(renderer, rt_ui->deepth.text, NULL, &rt_ui->deepth.rect);
	SDL_RenderCopy(renderer, rt_ui->raycast.text, NULL, &rt_ui->raycast.rect);
	SDL_RenderCopy(renderer, rt_ui->def.text, NULL, &rt_ui->def.rect);
	//SDL_RenderPresent(renderer);
	//my_button->name.draw(renderer, &(my_button->name));
}

SDL_Rect make_rect(int x, int y, int w, int h)
{
	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return(rect);
}

void	init_ui(t_sdl *sdl, t_scene *scene)
{
	scene->rt_ui = (t_ui *)protected_malloc(sizeof(t_ui), 1);
	scene->rt_ui->save_png = init_save_butt(sdl->renderer, make_rect(1400, 5, 70, 70), "icon.png");
	scene->rt_ui->ambiance = init_save_butt(sdl->renderer, make_rect(1450, 100, 200, 70), "ambiance.png");
	scene->rt_ui->am_plus = init_save_butt(sdl->renderer, make_rect(1550, 200, 50, 50), "plus.png");
	scene->rt_ui->am_minus = init_save_butt(sdl->renderer, make_rect(1450, 200, 50, 50), "minus.png");
	scene->rt_ui->modes = init_save_butt(sdl->renderer, make_rect(1450, 300, 200, 60), "modes.png");
	scene->rt_ui->normal = init_save_butt(sdl->renderer, make_rect(1450, 400, 40, 40), "1_icon.png");
	scene->rt_ui->deepth = init_save_butt(sdl->renderer, make_rect(1500, 400, 40, 40), "2_icon.png");
	scene->rt_ui->raycast = init_save_butt(sdl->renderer, make_rect(1550, 400, 40, 40), "3_icon.png");
	scene->rt_ui->def = init_save_butt(sdl->renderer, make_rect(1600, 400, 40, 40), "4_icon.png");
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
	SDL_CreateWindowAndRenderer(WID + 400, HEI, 0, &sdl.win, &sdl.renderer);
	SDL_RenderClear(sdl.renderer);
	//scene->obj_nmb += 1;
//	scene->objs[scene->obj_nmb - 1] = new_torus(get_point(0,0,100), 20, 100, set_color(0,0,255,255), get_point(0,0,0), 100);
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
	init_ui(&sdl, scene);
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
