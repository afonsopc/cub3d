/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:18:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 11:06:38 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	init_stats_hud_states(t_game *game)
{
	char		*key;
	t_sprite	*sprite;

	game->hud.stats.states = -1;
	while (++game->hud.stats.states || true)
	{
		key = ft_strf("STATS_HUD_%d", game->hud.stats.states + 1);
		sprite = ft_hashmap_get_value(game->sprites, key);
		free(key);
		if (!sprite)
			break ;
		ft_list_add(&game->hud.stats.states_list, sprite, NULL);
	}
	if (game->hud.stats.states)
		game->hud.stats.sprite = ft_list_index(
				game->hud.stats.states_list, 0)->data;
}

static void	init_stats_hud(t_game *game)
{
	game->hud.stats.cheat_sprite = ft_hashmap_get_value(game->sprites,
			"STATS_HUD_CHEAT");
	game->hud.stats.dead_sprite = ft_hashmap_get_value(game->sprites,
			"STATS_HUD_DEAD");
	game->hud.stats_enabled = !ft_strequal(ft_hashmap_get_value(
				game->map->types, "STATS_HUD"), "FALSE");
	game->hud.stats_font = ft_hashmap_get_value(game->fonts, "STATS_HUD");
	if (!game->hud.stats_font)
		game->hud.stats_font = ft_hashmap_get_value(game->fonts, "DEFAULT");
	game->hud.stats.prev_health = -1;
	init_stats_hud_states(game);
}

static void	init_debug_hud(t_game *game)
{
	game->hud.debug_enabled = ft_strequal(ft_hashmap_get_value(game->map->types,
				"DEBUG_HUD"), "TRUE");
	game->hud.debug_font = ft_hashmap_get_value(game->fonts, "DEBUG_HUD");
	if (!game->hud.debug_font)
		game->hud.debug_font = ft_hashmap_get_value(game->fonts, "DEFAULT");
}

static void	init_action_hud(t_game *game)
{
	game->hud.action_enabled = !ft_strequal(ft_hashmap_get_value(
				game->map->types, "ACTION_HUD"), "FALSE");
	game->hud.action_font = ft_hashmap_get_value(game->fonts, "ACTION_HUD");
	if (!game->hud.action_font)
		game->hud.action_font = ft_hashmap_get_value(game->fonts, "DEFAULT");
}

void	init_hud(t_game *game)
{
	game->hud.enabled = !ft_strequal(ft_hashmap_get_value(game->map->types,
				"HUD"), "FALSE");
	game->hud.minimap_enabled = ft_strequal(ft_hashmap_get_value(
				game->map->types, "MINIMAP_HUD"), "TRUE");
	init_stats_hud(game);
	init_debug_hud(game);
	init_action_hud(game);
}
