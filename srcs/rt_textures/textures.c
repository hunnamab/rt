/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 18:09:19 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/22 05:05:34 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char				*get_file(char *description)
{
	char			*type;
	char			*buf;
	int				i;
	int				len;

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

t_texture			*tex_new_bmp(char *file)
{
	SDL_Surface		*s;
	char			*buf;

	if (ft_strequ(&file[ft_strlen(file) - 1], ","))
		buf = ft_strsub(file, 1, (ft_strlen(file) - 3));
	else
		buf = ft_strsub(file, 1, (ft_strlen(file) - 2));
	if ((s = IMG_Load(buf)) == NULL)
	{
		free(file);
		free(buf);
		return (NULL);
	}
	if (ft_strstr(file, "perlin_"))
	{
		free(file);
		free(buf);
		return (calc_perlin());
	}
	else
	{
		free(buf);
		free(file);
		return (tex_new_surface(s));
	}
}

t_texture			*tex_new_surface(SDL_Surface *s)
{
	t_texture		*ret;

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

t_color				get_color_tex(t_texture *texture, float x,
						float y, int index)
{
	t_color			c;
	Uint8			bpp;
	unsigned int	pixel;
	int				fx;
	int				fy;

	index = 0;
	if (x < 0)
		x = texture->width + x;
	if (y < 0)
		y = texture->height + y;
	fx = texture->width - (int)(texture->width * x) % texture->width;
	fy = (int)(texture->height * y) % texture->height;
	bpp = texture->bytes_per_pixel;
	pixel = *(unsigned int *)(((unsigned char*)texture->pixels) +
						bpp * fx + texture->l_size * fy);
	c.red = pixel >> 16;
	c.green = pixel >> 8;
	c.blue = pixel >> 0;
	return (c);
}
