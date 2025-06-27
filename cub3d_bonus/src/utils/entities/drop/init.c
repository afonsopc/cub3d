/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:59:43 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "drop.h"

static void	set_item(t_game *game, t_ftm_window *window, t_drop *drop,
	char identifier)
{
	t_type_creator	item_creator;
	char			*item_type;

	item_type = hashmap_get_with_identifier(game, game->map->types, identifier,
			"ITEM");
	if (!item_type)
		return ;
	item_creator = get_type_creator(game->map->identifiers, item_type[0]);
	if (!item_creator)
		return ;
	drop->item = item_creator(game, window, item_type[0]);
}

void	init_drop(t_game *game, t_ftm_window *window, t_drop *drop,
	char identifier)
{
	init_billboard(game, window, (t_billboard *)drop, identifier);
	drop->billboard.entity.type = ENTITY_DROP;
	drop->billboard.entity.frame = drop_frame;
	drop->billboard.entity.clear = clear_drop;
	drop->billboard.entity.action = drop_action;
	drop->billboard.entity.shot = drop_shot;
	drop->billboard.entity.hard = false;
	drop->auto_use = ft_strequal(hashmap_get_with_identifier(game,
				game->map->types, identifier, "AUTO_USE"), "TRUE");
	drop->auto_pickup = ft_strequal(hashmap_get_with_identifier(game,
				game->map->types, identifier, "AUTO_PICKUP"), "TRUE");
	set_item(game, window, drop, identifier);
}

t_drop	*drop_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_drop	*drop;

	drop = ft_calloc(1, sizeof(t_drop));
	if (!drop)
		return (NULL);
	init_drop(game, window, drop, identifier);
	return (drop);
}
