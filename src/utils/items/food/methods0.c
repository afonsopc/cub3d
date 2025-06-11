/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 19:03:20 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "food.h"

void	clear_food(void *data)
{
	clear_item(data);
}

void	food_use(t_item *item)
{
	item_use(item);
	if (!item->user)
		return ;
	item->user->billboard.entity.health += ((t_food *)item)->health;
	if (item->user->billboard.entity.health > item->user->billboard.entity.max_health)
		item->user->billboard.entity.health = item->user->billboard.entity.max_health;
	if (item->user->billboard.entity.health < 0)
		item->user->billboard.entity.health = 0;
}

void	food_frame(t_item *item)
{
	item->can_use = !item->last_use;
	item_frame(item);
}

