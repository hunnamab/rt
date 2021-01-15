/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 18:09:19 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/01/15 15:44:29 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char		*get_file(char *description)
{
	char	*type;
	char	*buf;
	int		i;
	int		len;

	if (!description)
		output_error(5);
	i = 0;
	len = ft_strlen(description);
	while (description[i] != ':' && description[i] != '\0')
		i++;
	i++;
	if (i > len)
		output_error(5);
	type = ft_strsub(description, i, len - i);
	buf = ft_strtrim(type);
	free(type);
	return (buf);
}

t_texture		*tex_new_bmp(char *file)
{
	SDL_Surface	*s;

	file = ft_strsub(file, 1, (ft_strlen(file) - 2));
	if ((s = IMG_Load(file)) == NULL)
		return (NULL);
	return (tex_new_surface(s));
}

t_texture		*tex_new_surface(SDL_Surface *s)
{
	t_texture	*ret;

	ret = (t_texture *)malloc(sizeof(t_texture));
	ret->img = s;
	ret->pixels = s->pixels;
	ret->width = s->w;
	ret->height = s->h;
	ret->bytes_per_pixel = s->format->BytesPerPixel;
	ret->l_size = s->pitch;
	ret->size = ret->height * ret->l_size;
	ret->format = s->format;
	return (ret);
}

t_color	get_color_tex(t_texture *texture, float x, float y)
{

	if (x < 0)
		x = texture->width + x;
	if (y < 0)
		y = texture->height + y;
	int fx = texture->width - (int)(texture->width * x) % texture->width;
	int fy = (int)(texture->height * y) % texture->height;
   	Uint8 bpp = texture->bytes_per_pixel;
	Uint32 pixel = *(Uint32 *)(((Uint8*)texture->pixels) + bpp * fx + texture->l_size * fy);
    t_color c;
	c.red = pixel >> 16;
	c.green = pixel >> 8;
	c.blue = pixel >> 0;
    return (c);
}