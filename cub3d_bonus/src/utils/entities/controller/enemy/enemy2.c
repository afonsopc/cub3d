/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:50:38 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 21:23:16 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enemy.h"

bool	do_has_seen_in_short_notice(t_controller *controller,
	t_character *character)
{
	controller->walking_forward = true;
	if (ft_get_time() - controller->last_seen_target >= 2000)
	{
		character->billboard.entity.coords.yaw
			= ft_normalize_angle(character->billboard.entity.coords.yaw
				+ ft_angle_distance(character->billboard.entity.coords,
					controller->prev_target->coords));
		return (true);
	}
	character->billboard.entity.coords.yaw = ft_normalize_angle(
			character->billboard.entity.coords.yaw + ft_angle_distance(
				character->billboard.entity.coords,
				controller->last_target_position));
	if (ft_distance(character->billboard.entity.coords,
			controller->last_target_position) > 0.25)
		return (true);
	controller->walking_forward = false;
	return (false);
}

void	look_around(t_character *character, t_controller *controller)
{
	if (controller->prev_target
		&& controller->prev_target->character
		&& !((t_character *)controller->prev_target)->dead
		&& ft_get_time() - controller->last_seen_target < 10000
		&& do_has_seen_in_short_notice(controller, character))
		return ;
	if ((double)rand() / RAND_MAX < LOOK_AROUND_CHANCE)
	{
		if (ft_normalize_angle(controller->moving_to_angle
				- controller->prev_angle) < 270)
			controller->moving_to_angle
				= ft_normalize_angle(controller->prev_angle - 90.0);
		else
			controller->moving_to_angle
				= ft_normalize_angle(controller->prev_angle + 90.0);
	}
	if (controller->moving_to_angle == character->billboard.entity.coords.yaw)
		return ;
	if (ft_normalize_angle(controller->moving_to_angle
			- character->billboard.entity.coords.yaw) > 180)
		controller->looking_left = true;
	else
		controller->looking_right = true;
}

void	do_has_heard(t_game *game, t_character *character, t_player *player)
{
	character->billboard.entity.coords.yaw
		= ft_normalize_angle(character->billboard.entity.coords.yaw
			+ ft_angle_distance(
				character->billboard.entity.coords,
				player->character.billboard.entity.coords));
	targets_frame(game, character, 120);
}

void	hearment(t_game *game, t_character *character)
{
	t_player	*player;
	t_item		*item;
	double		dist;
	int			i;

	i = -1;
	while (game->billboards[++i])
	{
		if (!game->billboards[i]
			|| game->billboards[i]->type != ENTITY_PLAYER)
			continue ;
		player = (t_player *)game->billboards[i];
		item = player->character.inventory[player->character.inventory_index];
		dist = ft_distance(character->billboard.entity.coords,
				game->billboards[i]->coords);
		if (!(game->billboards[i]->controller.sprinting
				&& dist < SPRINT_HEARING_RANGE)
			&& !(item && item->user && dist < SHOT_HEARING_RANGE))
			continue ;
		do_has_heard(game, character, player);
		return ;
	}
}

void	shoot(t_character *character)
{
	t_item	*item;

	item = character->inventory[character->inventory_index];
	if (!item || !character->target_entity
		|| (item->weapon && ((t_weapon *)item)->range > 0
			&& ft_distance(character->billboard.entity.coords,
				character->target_entity->coords) > ((t_weapon *)item)->range))
		return ;
	if (character->inventory_index == 0
		&& character->inventory_index < INVENTORY_SIZE - 1
		&& character->inventory[character->inventory_index + 1]
		&& ft_get_time() - item->last_use >= item->use_delay)
	{
		item->user = NULL;
		++character->inventory_index;
		item = character->inventory[character->inventory_index];
	}
	item->user = character;
}
