/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:00:14 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 16:04:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "drop.h"

void	set_sprite(t_drop	*drop)
{
	if (!drop->item)
		return ;
	if (drop->prev_item == drop->item)
		return ;
	drop->prev_item = drop->item;
	fill_3d_sprites_from_single(drop->billboard.sprites,
		drop->item->icon_sprite);
}

bool	add_ammo_if_already_has_the_weapon(t_character *character,
	t_weapon *weapon)
{
	int			i;
	t_weapon	*inv_weapon;

	i = -1;
	character->ammo += weapon->ammo_usage * 10;
	while (++i < INVENTORY_SIZE)
	{
		inv_weapon = (t_weapon *)character->inventory[i];
		if (!inv_weapon || !inv_weapon->item.weapon)
			continue ;
		if (inv_weapon->item.identifier != weapon->item.identifier)
			continue ;
		return (true);
	}
	return (false);
}

static bool	bill_didnt_collided(t_entity *entity, t_drop *drop)
{
	t_dsize		e_size;
	t_coords	d_coords;

	e_size = entity->size;
	d_coords = drop->billboard.entity.coords;
	return (entity == (t_entity *)drop || entity->type != ENTITY_PLAYER
		|| d_coords.x + e_size.width < entity->coords.x
		|| d_coords.x - e_size.width > (entity->coords.x + e_size.width)
		|| d_coords.y + e_size.height < entity->coords.y
		|| d_coords.y - e_size.height > (entity->coords.y + e_size.height));
}

void	do_the_thing(t_game *game, t_drop *drop)
{
	int			i;
	t_entity	*entity;

	if (!drop->item || (!drop->auto_use && !drop->auto_pickup))
		return ;
	i = -1;
	while (game->billboards[++i])
	{
		entity = game->billboards[i];
		if (bill_didnt_collided(entity, drop))
			continue ;
		if (drop->auto_use)
		{
			drop->item->user = (t_character *)entity;
			drop->item->use(drop->item, drop);
			drop->billboard.entity.active = false;
		}
		else if (drop->auto_pickup)
		{
			if (!drop->item->weapon || !add_ammo_if_already_has_the_weapon(
					(t_character *)entity, (t_weapon *)drop->item))
				add_item_to_inventory((t_character *)entity, drop->item);
			drop->billboard.entity.active = false;
		}
	}
}
