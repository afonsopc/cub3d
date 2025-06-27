/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:43:54 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 02:38:23 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_player_canvas_data	get_player_canvas_data_2(t_size cs,
	int player_index)
{
	if (player_index == 0 || player_index == 1)
		return ((t_player_canvas_data){{cs.width / 2, cs.height / 2},
			{player_index * cs.width / 2, 0, 0}});
	if (player_index == 2 || player_index == 3)
		return ((t_player_canvas_data){{cs.width / 2, cs.height / 2},
			{(player_index - 2) * cs.width / 2, cs.height / 2, 0}});
	return ((t_player_canvas_data){0});
}

static t_player_canvas_data	get_player_canvas_data(t_game *game, t_size cs,
	int player_index)
{
	int	i;

	i = -1;
	while (++i < PLAYER_MAX)
		if (!game->players[i] || !((t_entity *)game->players[i])->active)
			break ;
	if (player_index >= i)
		return ((t_player_canvas_data){{cs.width / 2, cs.height / 2},
			{0, cs.height / 2, 0}});
	if (i == 1)
		return ((t_player_canvas_data){cs, {0, 0, 0}});
	if (i == 2)
		return ((t_player_canvas_data){{cs.width / 2, cs.height / 2},
			{player_index * (cs.width / 2), (cs.height / 2) / 2, 0}});
	if (i == 3)
	{
		if (player_index == 0 || player_index == 1)
			return ((t_player_canvas_data){{cs.width / 2, cs.height / 2},
				{player_index * cs.width / 2, 0, 0}});
		return ((t_player_canvas_data){{cs.width / 2, cs.height / 2},
			{(cs.width / 2) / 2, cs.height / 2, 0}});
	}
	return (get_player_canvas_data_2(cs, player_index));
}

static bool	canvas_changed(t_player *player, t_player_canvas_data pcd)
{
	return (!player->canvas || player->canvas->size.width != pcd.size.width
		|| player->canvas->size.height != pcd.size.height
		|| player->last_canvas_pos.x != pcd.coords.x
		|| player->last_canvas_pos.y != pcd.coords.y);
}

void	render_players_game(t_game *game, t_ftm_window *window)
{
	t_player_canvas_data	player_canvas_data;
	int						i;

	i = -1;
	while (++i < PLAYER_MAX)
	{
		if (!game->players[i] || !((t_entity *)game->players[i])->active)
			return ;
		player_canvas_data = get_player_canvas_data(game, window->size, i);
		if (canvas_changed(game->players[i], player_canvas_data))
		{
			ftm_put_image_to_window_pitc(window, get_sprite_image(
					game->background_sprite), (t_ftm_pitc_config){.coords = {0},
				.crop = false, .resize = true, .size = window->size});
			ftm_free_image(game->players[i]->canvas);
			game->players[i]->canvas = ftm_image_new(window,
					player_canvas_data.size);
			game->players[i]->last_canvas_pos = player_canvas_data.coords;
			game->players[i]->character.rays = player_canvas_data.size.width;
		}
		render_game(game, game->players[i]->canvas,
			(t_character *)game->players[i]);
		ftm_put_image_to_window(window, game->players[i]->canvas,
			player_canvas_data.coords);
	}
}
