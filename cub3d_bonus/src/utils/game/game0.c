/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:27:08 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 20:14:58 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	init_hashmaps_e(t_game *game)
{
	fte_set(NULL);
	game->sprites = ft_hashmap_new();
	if (!game->sprites)
		return (fte_set("init sprites"));
	game->sounds = ft_hashmap_new();
	if (!game->sounds)
		return (fte_set("init sounds"));
	game->fonts = ft_hashmap_new();
	if (!game->fonts)
		return (fte_set("init sounds"));
	game->sprites_3d = ft_hashmap_new();
	if (!game->fonts)
		return (fte_set("init sprites 3d"));
}

static bool	init_game_basics(t_ftm_window *window, t_game *game)
{
	init_hashmaps_e(game);
	if (fte_flagged())
		return (false);
	init_sprites_e(window, game);
	if (fte_flagged())
		return (false);
	init_sprites_3d(game);
	init_enviroment_e(game);
	if (fte_flagged())
		return (false);
	init_minimap(window, game);
	init_sounds_e(game);
	if (fte_flagged())
		return (false);
	return (true);
}

static bool	init_game_entities(t_ftm_window *window, t_game *game)
{
	init_entities_e(game, window);
	if (fte_flagged())
		return (false);
	init_fonts_e(window, game);
	if (fte_flagged())
		return (false);
	init_font_e(window, game);
	if (fte_flagged())
		return (false);
	update_walls_matrix(game);
	if (fte_flagged())
		return (false);
	insert_door_frames(game);
	update_billboards_vec(game);
	if (fte_flagged())
		return (false);
	return (true);
}

static void	finalize_game(t_ftm_window *window, t_game *game)
{
	init_hud(game);
	init_window(window, game);
	init_fps(game);
	init_threads_e(game);
	game->background_sound = ft_hashmap_get_value(game->sounds, "BACKGROUND");
	game->background_sprite = ft_hashmap_get_value(game->sprites, "BACKGROUND");
	if (game->players[0])
		game->players[0]->character.billboard.entity.active = true;
}

t_game	*game_new_e(t_ftm_window *window, t_map *map)
{
	t_game	*game;

	fte_set(NULL);
	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		return (fte_set("game new"), NULL);
	game->map = map;
	if (!init_game_basics(window, game))
		return (free_game(game), NULL);
	if (!init_game_entities(window, game))
		return (free_game(game), NULL);
	finalize_game(window, game);
	if (fte_flagged())
		return (free_game(game), NULL);
	return (game);
}
