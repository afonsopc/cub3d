/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 18:54:38 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "character.h"

static void	create_inventory_items(t_game *game, t_ftm_window *window, t_character *character)
{
	int				i;
	char			*key;
	char			*inv_item_type;
	t_item_creator	item_creator;

	i = -1;
	while (++i < INVENTORY_SIZE)
	{
		key = ft_strf("%d_INVENTORY", i);
		if (!key)
			continue ;
		inv_item_type = hashmap_get_with_identifier(game, game->map->types, character->billboard.entity.identifier, key);
		free(key);
		if (ft_strlen(inv_item_type) != 1)
			continue ;
		item_creator = get_item_creator(&game->map->identifiers, inv_item_type[0]);
		if (!item_creator)
			continue ;
		character->billboard.entity.inventory[i] = item_creator(game, window, inv_item_type[0]);
	}
}

void	init_character(t_game *game, t_ftm_window *window, t_character *character, char identifier)
{
	init_billboard(game, window, (t_billboard *)character, identifier);
	character->billboard.entity.type = ENTITY_CHARACTER;
	character->billboard.entity.frame = character_frame;
	character->billboard.entity.clear = clear_character;
	character->billboard.entity.action = character_action;
	character->billboard.entity.shot = character_shot;
	character->death_sprite = ft_calloc(360, sizeof(t_sprite *));
	character->walking_sprite = ft_calloc(360, sizeof(t_sprite *));
	character->hit_sprite = ft_calloc(360, sizeof(t_sprite *));
	character->billboard.entity.character = true;
	fill_3d_sprites_from_game(game, character->walking_sprite, identifier, "WALKING");
	fill_3d_sprites_from_game(game, character->death_sprite, identifier, "DEATH");
	fill_3d_sprites_from_game(game, character->hit_sprite, identifier, "HIT");
	character->_sprite = character->billboard.sprites;
	character->death_sound = hashmap_get_with_identifier(game, game->sounds, identifier, "DEATH");
	character->hit_sound = hashmap_get_with_identifier(game, game->sounds, identifier, "HIT");
	character->target_entity = (t_entity *)game->player;
	create_inventory_items(game, window, character);
}

t_character	*character_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_character	*character;

	character = ft_calloc(1, sizeof(t_character));
	if (!character)
		return (NULL);
	init_character(game, window, character, identifier);
	return (character);
}
