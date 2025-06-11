/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/11 12:26:31 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "item.h"

void	clear_item(void *data)
{
	(void)data;
}

void	item_use(t_item *item)
{
	if (!item->user)
		return ;
	item->last_use = ft_get_time();
	fta_play(item->use_sound);
}

void	item_frame(t_item *item)
{
	bool	can_use1;
	bool	can_use2;

	if (item->user && !item->can_use)
	{
		item->screen_sprite = item->_screen_sprite;
		if (item->cant_sound
			&& ft_get_time() - item->last_cant_use_sound_play >= item->cant_sound->length)
		{
			item->last_cant_use_sound_play = ft_get_time();
			fta_play(item->cant_sound);
		}
		return ;
	}
	can_use1 = !item->single_use && item->user
		&& item->use_sound
		&& item->screen_sprite
		&& ft_get_time() - item->last_use >= item->screen_sprite->update_delay * 2;
	can_use2 = item->single_use && !item->already_using && item->user;
	if ((can_use1 || can_use2) && item->use)
		item->use(item);
	if (((!item->single_use && item->user) || (item->single_use && !item->already_using && item->user)) && item->screen_use_sprite)
	{
		item->screen_sprite = item->screen_use_sprite;
		item->screen_sprite->index = 0;
		item->screen_sprite->running = true;
		item->screen_sprite->loop = !item->single_use;
		if (item->single_use)
			item->screen_sprite->updated_at = ft_get_time();
	}
	else if (!item->single_use
		|| ((item->screen_use_sprite && !item->screen_use_sprite->running) && !item->user))
		item->screen_sprite = item->_screen_sprite;
	item->already_using = !!item->user;
}
