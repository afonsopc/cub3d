/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:17:42 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:06:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "character.h"

void	set_using(t_game *game, t_character *character)
{
	char		*key;
	t_sprite	**original;
	t_item		*item;

	item = character->inventory[character->inventory_index];
	key = ft_strf("USING_%c", item->identifier);
	original = hashmap_get_with_identifier(game, game->sprites_3d,
			character->billboard.entity.identifier, key);
	free(key);
	if (!original)
		return ;
	if (character->billboard.sprites != character->using_sprite
		|| character->last_used_item_identifier != item->identifier)
		fill_3d_sprites_from_src(character->using_sprite, original);
	character->billboard.sprites = character->using_sprite;
	character->last_used_item_identifier = item->identifier;
}

bool	walking(t_entity *entity)
{
	return (entity->controller.walking_right
		|| entity->controller.walking_backward
		|| entity->controller.walking_forward
		|| entity->controller.walking_left);
}

bool	using(t_character *character)
{
	t_item	*item;
	bool	is_in_delay;

	item = character->inventory[character->inventory_index];
	if (!item)
		return (false);
	is_in_delay = item->screen_sprite
		&& ft_get_time() - item->last_use < item->screen_sprite->update_delay;
	return (is_in_delay || item->user == character);
}

void	call_item_frames(t_character *character)
{
	int	i;

	i = -1;
	while (++i < INVENTORY_SIZE)
		if (character->inventory[i] && character->inventory[i]->frame)
			character->inventory[i]->frame(character->inventory[i]);
}

void	drop_items(t_game *game, t_character *character)
{
	int		i;

	if (!character->drop_items)
		return ;
	if (!character->drop)
	{
		i = -1;
		while (++i < INVENTORY_SIZE)
			if (character->inventory[i] && character->inventory[i]->drop)
				character->inventory[i]->drop(game, cub3d()->window,
					character->inventory[i], character);
		return ;
	}
	((t_entity *)character->drop)->coords = ((t_entity *)character)->coords;
	ft_list_add(&game->entities, character->drop, free_entity);
	character->drop = NULL;
}
