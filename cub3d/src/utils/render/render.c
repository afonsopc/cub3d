/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:49:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/14 14:40:53 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	render_ceiling_and_floor(t_game *game, t_ftm_image *canvas)
{
	ftm_draw_rectangle(canvas, (t_coords){0, 0, 0, 0}, (t_size){W_WIDTH,
		W_HEIGHT / 2}, (t_ftm_rectangle){game->environment.ceiling_color,
		game->environment.ceiling_color, (t_size){1, 1}});
	ftm_draw_rectangle(canvas, (t_coords){0, W_HEIGHT / 2, 0, 0},
		(t_size){W_WIDTH, W_HEIGHT / 2},
		(t_ftm_rectangle){game->environment.floor_color,
		game->environment.floor_color, (t_size){1, 1}});
}

void	render_game(t_ftm_window *window, t_game *game)
{
	render_ceiling_and_floor(game, window->canvas);
	render_raycasting_mega(game, window->canvas);
	ftm_update_window(window);
}
