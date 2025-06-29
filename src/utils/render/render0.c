/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:49:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/29 22:31:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	render_ceiling_and_floor(t_game *game, t_ftm_image *canvas)
{
	ftm_draw_rectangle(canvas, (t_coords){0, 0, 0}, canvas->size,
		(t_ftm_rectangle){game->environment.ceiling_color, 0, (t_size){0, 0}});
	ftm_draw_rectangle(canvas, (t_coords){0, canvas->size.height / 2, 0},
		canvas->size, (t_ftm_rectangle){game->environment.floor_color,
			0, (t_size){0, 0}});
}

void	render_game(t_game *game, t_ftm_image *canvas, t_character *character)
{
	if (!game || !canvas || !character)
		return ;
	render_ceiling_and_floor(game, canvas);
	render_camera(game, canvas, character);
	render_hud(game, canvas, character);
}
