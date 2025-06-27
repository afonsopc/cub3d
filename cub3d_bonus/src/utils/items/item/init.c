/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 20:53:40 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "item.h"

static void	init_item_crazy_things(t_game *game, t_item *item, char identifier)
{
	item->use_delay = ft_atof(hashmap_get_with_identifier(game,
				game->map->types, identifier, "USE_DELAY")) * 1000;
	item->_icon_sprite = item->icon_sprite;
	item->_screen_sprite = item->screen_sprite;
	item->single_use = ft_strequal(hashmap_get_with_identifier(game,
				game->map->types, identifier, "SINGLE_USE"), "TRUE");
	item->use_sound = hashmap_get_with_identifier(game, game->sounds,
			identifier, "USE");
	item->get_sound = hashmap_get_with_identifier(game, game->sounds,
			identifier, "GET");
	item->cant_sound = hashmap_get_with_identifier(game, game->sounds,
			identifier, "CANT_USE");
}

void	init_item(t_game *game, t_ftm_window *window, t_item *item,
	char identifier)
{
	(void)window;
	if (!item)
		return ;
	item->clear = clear_item;
	item->frame = item_frame;
	item->use = item_use;
	item->drop = item_drop;
	item->identifier = identifier;
	item->can_use = true;
	item->name = hashmap_get_with_identifier(game, game->map->types, identifier,
			"NAME");
	item->description = hashmap_get_with_identifier(game, game->map->types,
			identifier, "DESCRIPTION");
	sprite_soft_copy(&item->icon_sprite, hashmap_get_with_identifier(game,
			game->sprites, identifier, "ICON"));
	sprite_soft_copy(&item->screen_sprite, hashmap_get_with_identifier(game,
			game->sprites, identifier, "SCREEN"));
	sprite_soft_copy(&item->icon_use_sprite, hashmap_get_with_identifier(game,
			game->sprites, identifier, "ICON_USE"));
	sprite_soft_copy(&item->screen_use_sprite, hashmap_get_with_identifier(game,
			game->sprites, identifier, "SCREEN_USE"));
	init_item_crazy_things(game, item, identifier);
}

t_item	*item_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_item	*item;

	item = ft_calloc(1, sizeof(t_item));
	if (!item)
		return (NULL);
	init_item(game, window, item, identifier);
	return (item);
}
