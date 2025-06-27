/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 20:50:57 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collectible.h"

void	clear_collectible(void *data)
{
	clear_item(data);
}

void	collectible_use(t_item *item, t_drop *drop)
{
	t_collectible	*collectible;

	item_use(item, drop);
	collectible = (t_collectible *)item;
	if (!item->user)
		return ;
	item->user->billboard.entity.health += collectible->health;
	if (item->user->billboard.entity.health
		> item->user->billboard.entity.max_health)
		item->user->billboard.entity.health
			= item->user->billboard.entity.max_health;
	if (item->user->billboard.entity.health < 0)
		item->user->billboard.entity.health = 0;
	item->user->ammo += collectible->ammo;
	if (item->user->ammo < 0)
		item->user->ammo = 0;
	item->user->score += collectible->score;
}

void	collectible_drop(t_game *game, t_ftm_window *window, t_item *item,
	t_character *dropper)
{
	item_drop(game, window, item, dropper);
}

void	collectible_frame(t_item *item)
{
	item->can_use = !item->last_use;
	item_frame(item);
}
