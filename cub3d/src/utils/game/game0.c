/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:27:08 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 13:36:51 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	clear_game(void *data)
{
	t_game	*game;

	game = data;
	if (!game)
		return ;
	ft_list_destroy(&game->entities);
	if (game->map)
	{
		ft_list_destroy(&game->map->identifiers.air);
		ft_list_destroy(&game->map->identifiers.wall);
		ft_list_destroy(&game->map->identifiers.player);
	}
	ft_hashmap_destroy(game->sprites);
	ft_bzero(game, sizeof(t_game));
}

void	game_load_map_e(t_game *game, t_ftm_window *window, t_map *map)
{
	fte_set(ERROR_NO_ERROR);
	if (!game)
		return (fte_set(ERROR_INVALID_ARGS));
	if (!map)
		return (fte_set(ERROR_INVALID_MAP));
	game->map = map;
	game->sprites = ft_hashmap_new();
	if (!game->sprites)
		return (fte_set(ERROR_INIT_SPRITES));
	init_sprites_e(window, game);
	if (fte_flagged())
		return ;
	init_enviroment_e(game);
	if (fte_flagged())
		return ;
	init_entities_e(game);
	if (fte_flagged())
		return ;
}
