/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game9.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:48:36 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 23:14:32 by afpachec         ###   ########.fr       */
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
	ft_hashmap_destroy(game->sprites);
	ft_hashmap_destroy(game->sprites_3d);
	kill_threads(game);
	ft_hashmap_destroy(game->fonts);
	ft_hashmap_destroy(game->sounds);
	ft_list_destroy(&game->hud.stats.states_list);
	free_walls(game);
	free(game->billboards);
	ft_bzero(game, sizeof(t_game));
}

void	free_game(void *game)
{
	if (!game)
		return ;
	clear_game(game);
	free(game);
}
