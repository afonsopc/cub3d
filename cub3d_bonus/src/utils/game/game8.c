/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game8.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 18:20:15 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 02:39:50 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static char	*get_key(char *key)
{
	if (ft_str_endswith(key, "_SOUND_VOLUME"))
		return (ft_strndup(key, ft_strlen(key) - 13));
	else if (ft_str_endswith(key, "_SOUND_LOOP"))
		return (ft_strndup(key, ft_strlen(key) - 11));
	return (NULL);
}

static void	set_config(t_game *game, t_element *element)
{
	t_fta_audio	*audio;
	char		*key;

	if (!element)
		return ;
	key = get_key(element->key);
	if (!key)
		return ;
	audio = ft_hashmap_get_value(game->sounds, key);
	if (ft_str_endswith(key, "_SOUND_VOLUME"))
		fta_audio_config(audio, (t_fta_audio_config){ft_atof(element->value),
			audio->config.loop});
	else if (ft_str_endswith(key, "_SOUND_LOOP"))
		fta_audio_config(audio, (t_fta_audio_config){audio->config.volume,
			ft_strequal(element->value, "TRUE")});
	free(key);
}

static void	set_audio_configs_e(t_game *game)
{
	t_element			*curr;

	fte_set(NULL);
	curr = game->map->types->table;
	while (curr)
	{
		set_config(game, curr);
		curr = curr->next;
	}
}

static void	init_sound(t_game *game, t_element *el)
{
	t_fta_audio	*audio;
	char		*key;

	if (!ft_str_endswith(el->key, "_SOUND"))
		return ;
	if (!ft_str_endswith(el->value, ".mp3"))
		return (fte_set("invalid audio format"));
	audio = fta_audio_new(el->value);
	if (!audio && fta_engine()->initialized)
		return (fte_set("audio load"));
	if (!audio)
		return ;
	key = ft_strndup(el->key, ft_strlen(el->key) - 6);
	(ft_hashmap_set(game->sounds, key, audio, fta_free_audio), free(key));
}

void	init_sounds_e(t_game *game)
{
	t_element	*curr;

	fte_set(NULL);
	curr = game->map->types->table;
	while (curr)
	{
		init_sound(game, curr);
		if (fte_flagged())
			return ;
		curr = curr->next;
	}
	set_audio_configs_e(game);
}
