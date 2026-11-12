/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 20:20:04 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elevator.h"

void	elevator_frame(t_game *game, t_entity *entity, double delta_time)
{
	wall_frame(game, entity, delta_time);
}

void	clear_elevator(void *data)
{
	clear_wall(data);
	if (data)
		free(((t_elevator *)data)->locked_text);
}

static void	lever_down(t_elevator *elevator)
{
	t_wall	*w;

	elevator->pulled = true;
	fta_play(cub3d()->game->keyring.lever_sound);
	if (!elevator->on_sprite)
		return ;
	w = &elevator->wall;
	(free(w->north_sprite), free(w->south_sprite));
	(free(w->east_sprite), free(w->west_sprite));
	w->north_sprite = NULL;
	w->south_sprite = NULL;
	w->east_sprite = NULL;
	w->west_sprite = NULL;
	sprite_soft_copy(&w->north_sprite, elevator->on_sprite);
	sprite_soft_copy(&w->south_sprite, elevator->on_sprite);
	sprite_soft_copy(&w->east_sprite, elevator->on_sprite);
	sprite_soft_copy(&w->west_sprite, elevator->on_sprite);
}

static bool	lever_locked(t_game *game, t_elevator *elevator)
{
	if (!elevator->key || game_has_key(game, elevator->key))
		return (false);
	if (ft_get_time() - game->keyring.toast_at > 1500)
	{
		fta_play(game->keyring.locked_sound);
		if (elevator->locked_text)
			game_toast(game, elevator->locked_text);
	}
	return (true);
}

void	elevator_action(t_entity *entity, t_character *actioner)
{
	t_elevator	*elevator;
	t_game		*game;

	if (!entity)
		return ;
	game = cub3d()->game;
	elevator = (t_elevator *)entity;
	if (!actioner || actioner->billboard.entity.type != ENTITY_PLAYER
		|| (elevator->gives && elevator->pulled))
		return ;
	if (lever_locked(game, elevator))
		return ;
	wall_action(entity, actioner);
	if (elevator->gives)
	{
		lever_down(elevator);
		elevator->wall.entity.actionable = !!elevator->map_path;
		game_give_key(game, elevator->gives, NULL, NULL);
		if (!elevator->map_path)
			return ;
	}
	else if (elevator->on_sprite && !elevator->pulled)
		lever_down(elevator);
	if (story_level_end(game, elevator->map_path))
		return ;
	cub3d()->new_map_path = elevator->map_path;
}

void	elevator_shot(t_entity *shooted, t_character *shooter)
{
	wall_shot(shooted, shooter);
}
