/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filters.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:32:34 by hunnamab          #+#    #+#             */
/*   Updated: 2021/02/19 06:53:47 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILTERS_H
# define FILTERS_H
# include "rt.h"
# include "types.h"

enum	e_filters
{
	SEPIA,
	GAUSS,
	GRAYSCALE,
	NEGATIVE,
	DEFAULT,
	SHARPEN,
	MAGIC
};

void	filters_init(t_filter_data *data);
void	gauss_filter(t_filter_data *data);
void	sepia_filter(t_filter_data *scene);
void	gray_scale(t_filter_data *data);
void	negative(t_filter_data *data);
void	sharpen_filter(t_filter_data *data);
void	magic_filter(t_filter_data *data);

#endif
