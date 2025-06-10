/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 18:39:42 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "weapon.h"

void	clear_weapon(void *data)
{
	clear_item(data);
}

void	weapon_use(t_item *item)
{
	t_weapon	*weapon;
	t_entity	*target_entity;

	item_use(item);
	weapon = (t_weapon *)item;
	item->user->ammo -= weapon->ammo_usage;
	if (!item->user || !item->user->character)
		return ;
	target_entity = ((t_character *)item->user)->target_entity;
	if (target_entity && target_entity->shot)
		target_entity->shot(target_entity, item->user);
}

void	weapon_frame(t_item *item)
{
	item->can_use = item->user && item->user->ammo - ((t_weapon *)item)->ammo_usage >= 0;
	item_frame(item);
}
