/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldeirdre <ldeirdre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 18:09:19 by ldeirdre          #+#    #+#             */
/*   Updated: 2021/02/11 22:25:44 by ldeirdre         ###   ########.fr       */
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
	int i = ft_strlen(file);

	if (ft_strequ(&file[i - 1],  ","))
		file = ft_strsub(file, 1, (ft_strlen(file) - 3));
	else
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

t_color	get_color_tex(t_texture *texture, float x, float y, int index)
{

	if (x < 0)
		x = texture->width + x;
	if (y < 0)
		y = texture->height + y;
	int fx = texture->width - (int)(texture->width * x) % texture->width;
	int fy = (int)(texture->height * y) % texture->height;
   	Uint8 bpp = texture->bytes_per_pixel;
	unsigned int pixel = *(unsigned int *)(((unsigned char*)texture->pixels) + bpp * fx + texture->l_size * fy);
    t_color c;
	
	c.red = pixel >> 16;
	c.green = pixel >> 8;
	c.blue = pixel >> 0;
	if (index == 1280 * 360 - 640)
	{
		printf("fx %d fy %d l_size %d width %d height %d\n", fx, fy, texture->l_size, texture->width, texture->height);
		printf("color (%hhd,%hhd,%hhd)\n", c.red, c.green, c.blue);
		printf("index %d\n", index);
		printf("texture in textures.c (%hhu,%hhu,%hhu,%hhu)\n", texture->pixels[index], texture->pixels[index + 1], texture->pixels[index + 2], texture->pixels[index + 3]);
		printf("pixel host %u\n", pixel);
		printf("index host %d\n", bpp * fx + texture->l_size * fy);
		printf("bpp %d\n", texture->bytes_per_pixel);
	}
    return (c);
}
