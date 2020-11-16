/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmetron <pmetron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:22:54 by hunnamab          #+#    #+#             */
/*   Updated: 2020/11/11 12:10:03 by pmetron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	output_description(void)
{
	ft_putstr("usage: ./rtv1 [scene_file] [mode]*\n\n");
	ft_putstr("*mode is an optional argument,");
	ft_putstr(" it should be [default], [normal], [depth] or [raycast]\n");
	ft_putstr("\t1. default or no argument after scene file's name means");
	ft_putstr(" the program draws the usual scene\n");
	ft_putstr("\t2. normal means the program draws a normal map of");
	ft_putstr(" scene's objects\n");
	ft_putstr("\t3. depth mode draws a depth map of the scene's objects\n");
	ft_putstr("\t4. raycast can draw a scene with no shadows or shiny effect");
	ft_putstr(" from light sources\n");
}

void	output_error(int tag)
{
	if (tag == 0)
		ft_putstr("Error: no objects in the scene file or invalid file\n");
	else if (tag == 1)
		ft_putstr("Error: no light sources in the scene file\n");
	else if (tag == 2)
		ft_putstr("Error: no camera in the scene file or too many cameras\n");
	else if (tag == 3)
		ft_putstr("Error: the scene file is empty\n");
	else if (tag == 4)
		ft_putstr("Error: invalid object's name\n");
	else if (tag == 5)
		ft_putstr("Error: invalid object's parameters\n");
	else if (tag == 6)
	{
		ft_putstr("Error: invalid file structure\n");
		ft_putstr("Check brackets, spaces, etc.\n");
	}
	else if (tag == 7)
		ft_putstr("Error: wrong drawing mode\n");
	exit(0);
}
