/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:47:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "item.h"

void	clear_item(void *data)
{
	t_item	*item;

	if (!data)
		return ;
	item = (t_item *)data;
	free(item->_screen_sprite);
	free(item->_icon_sprite);
	free(item->icon_use_sprite);
	free(item->screen_use_sprite);
}

void	item_use(t_item *item, t_drop *drop)
{
	if (!item->user)
		return ;
	item->last_use = ft_get_time();
	item->user->last_use = ft_get_time();
	if (drop)
		item->user->last_auto_use = ft_get_time();
	fta_play(item->use_sound);
}

void	item_drop(t_game *game, t_ftm_window *window, t_item *item,
	t_character *dropper)
{
	t_drop	*drop;
	int		i;

	if (!game || !window || !item || !dropper)
		return ;
	drop = drop_new(game, window, 0);
	if (!drop)
		return ;
	item->user = NULL;
	i = -1;
	while (++i < INVENTORY_SIZE)
		if (dropper->inventory[i] == item)
			dropper->inventory[i] = NULL;
	drop->item = item;
	drop->auto_pickup = true;
	drop->billboard.entity.coords = dropper->billboard.entity.coords;
	if (dropper->billboard.entity.coords.yaw < 90
		|| dropper->billboard.entity.coords.yaw > 270)
		drop->billboard.entity.coords.x += 1;
	else
		drop->billboard.entity.coords.x -= 1;
	ft_list_add(&game->entities, drop, free_entity);
}

static void	cant_use(t_item *item)
{
	item->screen_sprite = item->_screen_sprite;
	if (item->cant_sound
		&& ft_get_time() - item->last_cant_use_sound_play
		>= item->cant_sound->length)
	{
		item->last_cant_use_sound_play = ft_get_time();
		fta_play(item->cant_sound);
	}
}

void	item_frame(t_item *item)
{
	bool	can_use1;
	bool	can_use2;

	if (item->user && !item->can_use)
		return (cant_use(item));
	can_use1 = !item->single_use && item->user && item->use_sound
		&& item->screen_sprite
		&& ft_get_time() - item->last_use >= item->use_delay;
	can_use2 = item->single_use && !item->already_using && item->user;
	if ((can_use1 || can_use2) && item->use)
		item->use(item, NULL);
	if (item->screen_use_sprite && ((!item->single_use && item->user)
			|| (item->single_use && !item->already_using && item->user)))
	{
		item->screen_sprite = item->screen_use_sprite;
		item->screen_sprite->index = 0;
		item->screen_sprite->running = true;
		item->screen_sprite->loop = !item->single_use;
		if (item->single_use)
			item->screen_sprite->updated_at = ft_get_time();
	}
	else if (!item->single_use || ((item->screen_use_sprite
				&& !item->screen_use_sprite->running) && !item->user))
		item->screen_sprite = item->_screen_sprite;
	item->already_using = !!item->user;
}
