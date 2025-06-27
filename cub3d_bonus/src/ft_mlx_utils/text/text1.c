/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 00:29:28 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:42:35 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "text.h"

void	ftm_draw_text_in_box(t_ftm_image *canvas, t_ftm_font *font,
	t_ftm_text_config text_config, t_ftm_rectangle rectangle)
{
	int			width;
	t_coords	rectangle_coords;
	t_size		rectangle_size;

	width = ftm_get_text_size_prediction(font, text_config)
		+ text_config.height * 1.7;
	rectangle_coords = (t_coords){text_config.coords.x - text_config.height
		/ 1.5, text_config.coords.y - text_config.height / 1.6, 0};
	rectangle_size = (t_size){width, text_config.height * 2.3};
	ftm_draw_rectangle(canvas, rectangle_coords, rectangle_size, rectangle);
	ftm_draw_text(canvas, font, text_config);
}
