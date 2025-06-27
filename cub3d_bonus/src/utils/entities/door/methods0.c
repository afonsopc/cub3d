/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 18:54:33 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "door.h"

void	door_frame(t_game *game, t_entity *entity, double delta_time)
{
	t_door		*door;

	wall_frame(game, entity, delta_time);
	door = (t_door *)entity;
	door->wall.entity.hard = door->opening_sprite->index
		!= door->animation_frames - 1;
	door->wall.entity.transparent = door->opening_sprite->index;
	(void)get_sprite_image(door->opening_sprite);
	if (!door->opening_sprite->index && door->opening_sprite->index
		!= door->last_animation_index)
		fta_play(door->close_sound);
	door->last_animation_index = door->opening_sprite->index;
	if (door->auto_close_delay && ft_get_time() - door->last_opened_at
		>= door->auto_close_delay
		&& door->wall.entity.action && door->opened)
		door->wall.entity.action(entity, NULL);
}

void	clear_door(void *data)
{
	t_door	*door;

	if (data)
	{
		((t_wall *)data)->north_sprite = NULL;
		((t_wall *)data)->south_sprite = NULL;
		((t_wall *)data)->east_sprite = NULL;
		((t_wall *)data)->west_sprite = NULL;
	}
	clear_wall(data);
	if (!data)
		return ;
	door = (t_door *)data;
	free_sprite(door->opening_sprite);
	free(door->door_sprite);
}

void	door_action(t_entity *entity, t_character *actioner)
{
	t_door	*door;

	wall_action(entity, actioner);
	door = (t_door *)entity;
	door->opened = !door->opened;
	door->opening_sprite->reversed = !door->opened;
	door->opening_sprite->running = true;
	door->opening_sprite->updated_at = ft_get_time();
	if (!door->closeable)
		door->wall.entity.targetable = false;
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
