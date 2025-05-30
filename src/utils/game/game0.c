/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:27:08 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/30 19:58:47 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	clear_game(void *game)
{
	if (!game)
		return ;
	ft_list_destroy(&((t_game *)game)->entities);
	ft_hashmap_destroy(((t_game *)game)->sprites);
	kill_threads(game);
	ft_hashmap_destroy(((t_game *)game)->fonts);
	ft_bzero(game, sizeof(t_game));
}

void	free_game(void *game)
{
	if (!game)
		return ;
	clear_game(game);
	free(game);
}

void	game_load_map_e(t_game *game, t_ftm_window *window, t_map *map)
{
	fte_set(ERROR_NO_ERROR);
	game->map = map;
	game->sprites = ft_hashmap_new();
	if (!game->sprites)
		return (fte_set(ERROR_INIT_SPRITES));
	game->sounds = ft_hashmap_new();
	if (!game->sounds)
		return (fte_set(ERROR_INIT_SOUNDS));
	game->fonts = ft_hashmap_new();
	if (!game->fonts)
		return (fte_set(ERROR_INIT_FONTS));
	init_sprites_e(window, game);
	if (fte_flagged())
		return (clear_game(game));
	init_enviroment_e(game);
	if (fte_flagged())
		return (clear_game(game));
	init_minimap(window, game);
	init_sounds_e(game);
	if (fte_flagged())
		return (clear_game(game));
	init_entities_e(game, window);
	if (fte_flagged())
		return (clear_game(game));
	init_threads_e(game);
	if (fte_flagged())
		return (clear_game(game));
	init_fonts_e(window, game);
	if (fte_flagged())
		return (clear_game(game));
	init_font_e(window, game);
	if (fte_flagged())
		return (clear_game(game));
	init_walls_e(game);
	if (fte_flagged())
		return (clear_game(game));
	insert_door_frames(&cub3d()->game);
	init_billboards_e(game);
	if (fte_flagged())
		return (clear_game(game));
	init_camera(game);
	init_hud(game);
	init_window(window, game);
	init_fps(game);
}

void	game_start(t_game *game, t_ftm_window *window)
{
	window->loop_hook = loop;
	window->key_hook = key_hook;
	window->exit_hook = cub3d_exit;
	window->mouse_hook = mouse_hook;
	ftm_update_hooks(window);
	fta_play(ft_hashmap_get_value(game->sounds, "BACKGROUND"));
	ftm_window_loop(window);
}
