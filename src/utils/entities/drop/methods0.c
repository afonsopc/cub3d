/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/11 12:28:42 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "drop.h"

static void	set_sprite(t_drop	*drop)
{
	if (!drop->item)
		return ;
	if (drop->prev_item == drop->item)
		return ;
	drop->prev_item = drop->item;
	fill_3d_sprites_from_single(drop->billboard.sprites, drop->item->icon_sprite);
}

static bool	add_ammo_if_already_has_the_weapon(t_character *character, t_weapon *weapon)
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

static void	do_the_thing(t_game *game, t_drop *drop)
{
	int			i;
	t_entity	*entity;

	if (!drop->item || (!drop->auto_use && !drop->auto_pickup))
		return ;
	i = -1;
	while (game->billboards[++i])
	{
		entity = game->billboards[i];
        if (entity == (t_entity *)drop || entity->type != ENTITY_PLAYER
            || drop->billboard.entity.coords.x + entity->size.width < entity->coords.x
            || drop->billboard.entity.coords.x - entity->size.width > (entity->coords.x + entity->size.width)
            || drop->billboard.entity.coords.y + entity->size.height < entity->coords.y
            || drop->billboard.entity.coords.y - entity->size.height > (entity->coords.y + entity->size.height))
			continue ;
		if (drop->auto_use)
		{
			drop->item->user = (t_character *)entity;
			drop->billboard.entity.active = false;
		}
		else if (drop->auto_pickup)
		{
			if (!drop->item->weapon || !add_ammo_if_already_has_the_weapon((t_character *)entity, (t_weapon *)drop->item))
				add_item_to_inventory((t_character *)entity, drop->item);
			drop->billboard.entity.active = false;
		}
	}
}

void	drop_frame(t_entity *entity, double delta_time)
{
	t_drop	*drop;

	billboard_frame(entity, delta_time);
	if (!entity)
		return ;
		drop = (t_drop *)entity;
	if (drop->item && drop->item->frame)
		drop->item->frame(drop->item);
	if (!entity->active)
		return ;
	set_sprite(drop);
	do_the_thing(&cub3d()->game, drop);
}

void	clear_drop(void *drop)
{
	clear_billboard(drop);
	if (!drop)
		return ;
	free_item(((t_drop *)drop)->item);
}

void	drop_action(t_entity *entity, t_character *actioner)
{
	billboard_action(entity, actioner);
}

void	drop_shot(t_entity *shooted, t_character *shooter)
{
	billboard_shot(shooted, shooter);
}
