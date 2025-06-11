/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/11 12:27:19 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ammo.h"

void	clear_ammo(void *data)
{
	clear_item(data);
}

void	ammo_use(t_item *item)
{
	item_use(item);
	if (!item->user)
		return ;
	item->user->ammo += ((t_ammo *)item)->amount;
	if (item->user->ammo < 0)
		item->user->ammo = 0;
}

void	ammo_frame(t_item *item)
{
	item->can_use = !item->last_use;
	item_frame(item);
}

