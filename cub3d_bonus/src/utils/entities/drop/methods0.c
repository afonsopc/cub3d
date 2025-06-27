/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 16:00:09 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "drop.h"

void	drop_frame(t_game *game, t_entity *entity, double delta_time)
{
	t_drop	*drop;

	billboard_frame(game, entity, delta_time);
	if (!entity)
		return ;
	drop = (t_drop *)entity;
	if (drop->item && drop->item->frame)
		drop->item->frame(drop->item);
	if (!entity->active)
		return ;
	set_sprite(drop);
	do_the_thing(game, drop);
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
