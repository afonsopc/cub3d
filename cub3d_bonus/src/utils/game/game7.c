/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game7.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:20:04 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 18:47:23 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	set_update_delay(t_sprite *sprite, t_hashmap *types,
	const char *key)
{
	char	*update_delay_key;
	char	*update_delay_str;

	update_delay_key = ft_strf("%s_SPRITE_UPDATE_DELAY", key);
	update_delay_str = ft_hashmap_get_value(types, update_delay_key);
	free(update_delay_key);
	if (!update_delay_str)
		return ;
	sprite->update_delay = (t_time)(fabs(ft_atof(update_delay_str)) * 1000);
}

static void	set_loop(t_sprite *sprite, t_hashmap *types, const char *key)
{
	bool	loop;
	char	*loop_key;
	char	*loop_str;

	loop_key = ft_strf("%s_SPRITE_LOOP", key);
	loop_str = ft_hashmap_get_value(types, loop_key);
	free(loop_key);
	if (!loop_str)
		return ;
	loop = ft_strequal(loop_str, "TRUE");
	sprite->loop = loop;
}

static void	set_reversed(t_sprite *sprite, t_hashmap *types, const char *key)
{
	bool	reversed;
	char	*reversed_key;
	char	*reversed_str;

	reversed_key = ft_strf("%s_SPRITE_REVERSED", key);
	reversed_str = ft_hashmap_get_value(types, reversed_key);
	free(reversed_key);
	if (!reversed_str)
		return ;
	reversed = ft_strequal(reversed_str, "TRUE");
	sprite->reversed = reversed;
}

void	set_sprite_configs(t_game *game)
{
	t_element	*curr;
	t_element	*el;

	el = game->sprites->table;
	while (el)
	{
		curr = el;
		el = el->next;
		if (!curr->value)
			continue ;
		set_update_delay((t_sprite *)curr->value, game->map->types, curr->key);
		set_loop((t_sprite *)curr->value, game->map->types, curr->key);
		set_reversed((t_sprite *)curr->value, game->map->types, curr->key);
	}
}

void	init_fonts_e(t_ftm_window *window, t_game *game)
{
	t_ftm_font			*font;
	t_element			*el;
	t_element			*curr;
	char				*key;

	fte_set(NULL);
	el = game->map->types->table;
	while (el)
	{
		curr = el;
		el = el->next;
		if (!ft_str_endswith(curr->key, "_FONT"))
			continue ;
		font = ftm_font_new(window, curr->value);
		if (!font)
			return (fte_set("default font not found"));
		key = ft_strndup(curr->key, ft_strlen(curr->key) - 5);
		ft_hashmap_set(game->fonts, key, font, ftm_free_font);
		free(key);
	}
}
