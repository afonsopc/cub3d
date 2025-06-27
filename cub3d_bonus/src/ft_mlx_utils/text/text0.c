/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 00:29:28 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:42:54 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "text.h"

static unsigned int	get_text_color(t_ftm_text_config *text_config,
	unsigned int pixel)
{
	if (!pixel)
		return (pixel);
	return (text_config->color);
}

static t_ftm_pitc_config	get_pitc_config(t_ftm_text_config *text_config,
	t_ftm_image *character, int *x)
{
	int			scaled_width;

	scaled_width = (character->size.width * text_config->height)
		/ character->size.height;
	if (!*x)
		*x = text_config->coords.x;
	else
		*x += scaled_width + text_config->spacing;
	return ((t_ftm_pitc_config){
		(t_coords){(int)*x, (int)text_config->coords.y, 0},
		false,
		(t_coords){0, 0, 0},
		(t_coords){0, 0, 0},
		true,
		(t_size){scaled_width, text_config->height},
		text_config,
		(void *)get_text_color
	});
}

void	ftm_draw_text(t_ftm_image *canvas, t_ftm_font *font,
	t_ftm_text_config text_config)
{
	t_ftm_image	*img;
	int			x;
	int			i;

	if (!canvas || !font || !text_config.text)
		return ;
	x = 0;
	i = -1;
	while (text_config.text[++i])
	{
		img = font->characters[(unsigned char)text_config.text[i]];
		if (img)
			ftm_put_image_to_canvas(canvas, img,
				get_pitc_config(&text_config, img, &x));
	}
}

int	ftm_get_text_size_prediction(t_ftm_font *font,
	t_ftm_text_config text_config)
{
	int			x;
	t_ftm_image	*img;
	int			i;
	int			scaled_width;

	if (!font || !text_config.text)
		return (0);
	x = text_config.coords.x;
	i = -1;
	while (text_config.text[++i])
	{
		img = font->characters[(unsigned char)text_config.text[i]];
		if (!img)
			continue ;
		scaled_width = (img->size.width * text_config.height)
			/ img->size.height;
		x += scaled_width + text_config.spacing;
	}
	return (x - text_config.coords.x);
}
