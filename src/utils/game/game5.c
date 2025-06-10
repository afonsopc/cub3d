/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:18:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 18:14:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	set_audio_configs_e(t_game *game)
{
	t_fta_audio			*audio;
	t_element			*el;
	t_element			*curr;
	char				*key;

	fte_set(NULL);
	el = *game->map->types->table;
	while (el)
	{
		curr = el;
		el = el->next;
		key = NULL;
		if (ft_str_endswith(curr->key, "_SOUND_VOLUME"))
		{
			key = ft_strndup(curr->key, ft_strlen(curr->key) - 13);
			audio = ft_hashmap_get_value(game->sounds, key);
			free(key);
			if (!audio)
				continue ;
			fta_audio_config(audio, (t_fta_audio_config){ft_atof(curr->value), audio->config.loop});
			continue ;
		}
		else if (ft_str_endswith(curr->key, "_SOUND_LOOP"))
		{
			key = ft_strndup(curr->key, ft_strlen(curr->key) - 11);
			audio = ft_hashmap_get_value(game->sounds, key);
			free(key);
			if (!audio)
				continue ;
			fta_audio_config(audio, (t_fta_audio_config){audio->config.volume, ft_strequal(curr->value, "TRUE")});
			continue ;
		}
		free(key);
	}
}

void	init_sounds_e(t_game *game)
{
	t_fta_audio			*audio;
	t_element			*el;
	t_element			*curr;
	char				*key;

	fte_set(NULL);
	el = *game->map->types->table;
	while (el)
	{
		curr = el;
		el = el->next;
		if (!ft_str_endswith(curr->key, "_SOUND"))
			continue ;
		if (!ft_str_endswith(curr->value, ".mp3"))
			return (fte_set("invalid audio format"));
		audio = fta_audio_new(curr->value);
		if (!audio && fta_engine()->initialized)
			return (fte_set("audio load"));
		if (!audio)
			continue ;
		key = ft_strndup(curr->key, ft_strlen(curr->key) - 6);
		ft_hashmap_set(game->sounds, key, audio, fta_free_audio);
		free(key);
	}
	set_audio_configs_e(game);
	if (fte_flagged())
		return ;
}

void	init_camera(t_game *game, t_ftm_window *window)
{
	game->camera.character = (t_character *)game->player;
	game->camera.rays = window->canvas->size.width;
	game->camera.fov = PLAYER_FOV;
}

static void	init_stats_hud(t_game *game)
{
	t_sprite	*sprite;
	char		*key;

	game->hud.stats.enabled = ft_strequal(ft_hashmap_get_value(game->map->types, "STATS_HUD"), "TRUE");
	game->hud.stats.font = ft_hashmap_get_value(game->fonts, "STATS_HUD");
	if (!game->hud.stats.font)
		game->hud.stats.font = ft_hashmap_get_value(game->fonts, "DEFAULT");
	game->hud.stats.prev_health = -1;
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
		game->hud.stats.sprite = ft_list_index(game->hud.stats.states_list, 0)->data;
}

static void	init_debug_hud(t_game *game)
{
	game->hud.debug.enabled = ft_strequal(ft_hashmap_get_value(game->map->types, "DEBUG_HUD"), "TRUE");
	game->hud.debug.font = ft_hashmap_get_value(game->fonts, "DEBUG_HUD");
	if (!game->hud.debug.font)
		game->hud.debug.font = ft_hashmap_get_value(game->fonts, "DEFAULT");
}

static void	init_action_hud(t_game *game)
{
	game->hud.action.enabled = ft_strequal(ft_hashmap_get_value(game->map->types, "ACTION_HUD"), "TRUE");
	game->hud.action.font = ft_hashmap_get_value(game->fonts, "ACTION_HUD");
	if (!game->hud.action.font)
		game->hud.action.font = ft_hashmap_get_value(game->fonts, "DEFAULT");
}

void	init_hud(t_game *game)
{
	game->hud.enabled = ft_strequal(ft_hashmap_get_value(game->map->types, "HUD"), "TRUE");
	init_stats_hud(game);
	init_debug_hud(game);
	init_action_hud(game);
}
