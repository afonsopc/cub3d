/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 16:50:19 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "item.h"

void	init_item(t_game *game, t_ftm_window *window, t_item *item,
	char identifier)
{
	(void)window;
	if (!item)
		return ;
	item->clear = clear_item;
	item->frame = item_frame;
	item->use = item_use;
	item->identifier = identifier;
	item->name = hashmap_get_with_identifier(game, game->map->types, identifier,
		"NAME");
	item->description = hashmap_get_with_identifier(game, game->map->types,
		identifier, "DESCRIPTION");
	item->icon_sprite = hashmap_get_with_identifier(game, game->sprites,
		identifier, "ICON");
	item->screen_sprite = hashmap_get_with_identifier(game, game->sprites,
		identifier, "SCREEN");
	item->icon_use_sprite = hashmap_get_with_identifier(game, game->sprites,
		identifier, "ICON_USE");
	item->screen_use_sprite = hashmap_get_with_identifier(game, game->sprites,
		identifier, "SCREEN_USE");
	item->_icon_sprite = item->icon_sprite;
	item->_screen_sprite = item->screen_sprite;
	item->single_use = ft_strequal(hashmap_get_with_identifier(game, game->map->types,
		identifier, "SINGLE_USE"), "TRUE");
	item->use_sound = hashmap_get_with_identifier(game, game->sounds,
		identifier, "USE");
	item->cant_sound = hashmap_get_with_identifier(game, game->sounds,
		identifier, "CANT_USE");
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
