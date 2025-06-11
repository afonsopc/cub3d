/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 21:36:09 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "door.h"

void	door_frame(t_entity *entity, double delta_time)
{
	t_door		*door;

	wall_frame(entity, delta_time);
	door = (t_door *)entity;
	door->wall.entity.hard = door->opening_sprite.index != DOOR_ANIMATION_FRAMES - 1;
	door->wall.entity.transparent = door->opening_sprite.index;
	(void)get_sprite_image(&door->opening_sprite);
	if (!door->opening_sprite.index && door->opening_sprite.index != door->last_animation_index)
		fta_play(door->close_sound);
	door->last_animation_index = door->opening_sprite.index;
	if (ft_get_time() - door->last_opened_at >= door->auto_close_delay
		&& door->wall.entity.action && door->opened)
		door->wall.entity.action(entity, NULL);
}

void	clear_door(void *door)
{
	clear_wall(door);
	clear_sprite(&((t_door *)door)->opening_sprite);
}

void	door_action(t_entity *entity, t_character *actioner)
{
	t_door	*door;

	wall_action(entity, actioner);
	door = (t_door *)entity;
	door->opened = !door->opened;
	door->opening_sprite.reversed = !door->opened;
	door->opening_sprite.running = true;
	door->opening_sprite.updated_at = ft_get_time();
	if (door->opened)
	{
		door->last_opened_at = ft_get_time();	
		fta_play(door->open_sound);
	}
}

void	door_shot(t_entity *shooted, t_character *shooter)
{
	wall_shot(shooted, shooter);
}
