/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:03:13 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/27 20:08:37 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

int	render_debug_str(t_game *game, t_ftm_image *canvas, char *str,
	t_coords coords)
{
	t_ftm_text_config	config;
	int					width;

	if (!str)
		return (0);
	config = (t_ftm_text_config){
		str,
		(t_coords){coords.x + 10, coords.y + 10, 0},
		16,
		-5,
		0xFFFFFFFF
	};
	width = ftm_get_text_size_prediction(game->hud.debug.font, config) + 25;
	ftm_draw_text_in_box(canvas, game->hud.debug.font, config,
		(t_ftm_rectangle){0x55000000, 0x55EFBF04, (t_size){2, 2}});
	return (width);
}

static void	set_strs(t_game *game)
{
	set_fps(game);
	set_target(game);
	set_player(game);
	set_entities(game);
}

static void	render_debug_strs(t_game *game, t_ftm_image *canvas)
{
	render_debug_fps_strs(game, canvas, (t_coords){20, 20, 0});
	render_debug_target_strs(game, canvas, (t_coords){20, 60, 0});
	render_debug_player_strs(game, canvas, (t_coords){20, 100, 0});
	render_debug_entities_strs(game, canvas, (t_coords){20, 140, 0});
}

void	render_debug(t_game *game, t_ftm_image *canvas)
{
	if (!game->hud.debug.enabled)
		return ;
	set_strs(game);
	render_debug_strs(game, canvas);
}
