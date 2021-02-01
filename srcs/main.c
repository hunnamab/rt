/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 15:39:02 by hunnamab          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/01/30 19:59:26 by pmetron          ###   ########.fr       */
=======
/*   Updated: 2021/01/30 20:16:04 by npetrell         ###   ########.fr       */
>>>>>>> 3d136036b906f8cafbd282cc7bd5a9dd32b83a5e
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
	SDL_RenderCopy(renderer, rt_ui->back.text, NULL, &rt_ui->back.rect);
	SDL_RenderCopy(renderer, rt_ui->save_png.text, NULL, &rt_ui->save_png.rect);
	SDL_RenderCopy(renderer, rt_ui->ambiance.text, NULL, &rt_ui->ambiance.rect);
	SDL_RenderCopy(renderer, rt_ui->am_plus.text, NULL, &rt_ui->am_plus.rect);
	SDL_RenderCopy(renderer, rt_ui->am_minus.text, NULL, &rt_ui->am_minus.rect);
	SDL_RenderCopy(renderer, rt_ui->modes.text, NULL, &rt_ui->modes.rect);
	SDL_RenderCopy(renderer, rt_ui->normal.text, NULL, &rt_ui->normal.rect);
	SDL_RenderCopy(renderer, rt_ui->deepth.text, NULL, &rt_ui->deepth.rect);
	SDL_RenderCopy(renderer, rt_ui->raycast.text, NULL, &rt_ui->raycast.rect);
	SDL_RenderCopy(renderer, rt_ui->def.text, NULL, &rt_ui->def.rect);
	SDL_RenderCopy(renderer, rt_ui->left.text, NULL, &rt_ui->left.rect);
	SDL_RenderCopy(renderer, rt_ui->right.text, NULL, &rt_ui->right.rect);
	SDL_RenderCopy(renderer, rt_ui->filters.text, NULL, &rt_ui->filters.rect);
	if (rt_ui->filt == 3)
		SDL_RenderCopy(renderer, rt_ui->gauss.text, NULL, &rt_ui->gauss.rect);
	if (rt_ui->filt == 1)
		SDL_RenderCopy(renderer, rt_ui->sepia.text, NULL, &rt_ui->sepia.rect);
	if (rt_ui->filt == 2)
		SDL_RenderCopy(renderer, rt_ui->grey.text, NULL, &rt_ui->grey.rect);
	if (rt_ui->filt == 0)
		SDL_RenderCopy(renderer, rt_ui->fault.text, NULL, &rt_ui->fault.rect);
	if (rt_ui->filt == 4)
		SDL_RenderCopy(renderer, rt_ui->negative.text, NULL, &rt_ui->negative.rect);
	if (rt_ui->filt == 5)
		SDL_RenderCopy(renderer, rt_ui->magic.text, NULL, &rt_ui->magic.rect);
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
	scene->rt_ui->save_png = init_save_butt(sdl->renderer, make_rect(1345, 10, 70, 70), "rt_ui/icon.png");
	scene->rt_ui->ambiance = init_save_butt(sdl->renderer, make_rect(1355, 100, 200, 70), "rt_ui/ambiance.png");
	scene->rt_ui->am_plus = init_save_butt(sdl->renderer, make_rect(1355, 200, 50, 50), "rt_ui/plus.png");
	scene->rt_ui->am_minus = init_save_butt(sdl->renderer, make_rect(1505, 200, 50, 50), "rt_ui/minus.png");
	scene->rt_ui->modes = init_save_butt(sdl->renderer, make_rect(1355, 300, 200, 60), "rt_ui/modes.png");
	scene->rt_ui->normal = init_save_butt(sdl->renderer, make_rect(1355, 400, 40, 40), "rt_ui/1_icon.png");
	scene->rt_ui->deepth = init_save_butt(sdl->renderer, make_rect(1408, 400, 40, 40), "rt_ui/2_icon.png");
	scene->rt_ui->raycast = init_save_butt(sdl->renderer, make_rect(1461, 400, 40, 40), "rt_ui/3_icon.png");
	scene->rt_ui->def = init_save_butt(sdl->renderer, make_rect(1513, 400, 40, 40), "rt_ui/4_icon.png");
	scene->rt_ui->back = init_save_butt(sdl->renderer, make_rect(1280, 0, 350, 720), "rt_ui/black.png");
	scene->rt_ui->left = init_save_butt(sdl->renderer, make_rect(1355, 600, 45, 45), "rt_ui/left.png");
	scene->rt_ui->right = init_save_butt(sdl->renderer, make_rect(1515, 600, 45, 45), "rt_ui/right.png");
	scene->rt_ui->filters = init_save_butt(sdl->renderer, make_rect(1355, 500, 200, 70), "rt_ui/filters.png");
	scene->rt_ui->gauss = init_save_butt(sdl->renderer, make_rect(1405, 600, 100, 40), "rt_ui/gauss.png");
	scene->rt_ui->sepia = init_save_butt(sdl->renderer, make_rect(1410, 600, 100, 40), "rt_ui/sepia.png");
	scene->rt_ui->grey = init_save_butt(sdl->renderer, make_rect(1410, 600, 100, 40), "rt_ui/grey.png");
	scene->rt_ui->fault = init_save_butt(sdl->renderer, make_rect(1410, 600, 100, 40), "rt_ui/default.png");
	scene->rt_ui->negative = init_save_butt(sdl->renderer, make_rect(1410, 600, 100, 40), "rt_ui/negative.png");
	scene->rt_ui->magic = init_save_butt(sdl->renderer, make_rect(1410, 600, 100, 40), "rt_ui/magic.png");
	scene->rt_ui->filt = 0;
	scene->rt_ui->i = 7;
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
	//scene->obj_nmb += 1;
	//scene->objs[scene->obj_nmb - 1] = new_torus(get_point(0,0,100), 100, 50, set_color(0,0,255,255), get_point(0,0,0), 100); */
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
	load_textures(scene);
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
