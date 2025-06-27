/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moviment0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:36:49 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:37:16 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "moviment.h"

static void	player_walk(t_list *entities, t_entity *entity, double angle,
	double delta_time)
{
	double		velocity;

	velocity = entity->controller.walk_velocity;
	if (entity->controller.sprinting)
		velocity = entity->controller.sprint_velocity;
	velocity *= delta_time;
	move_player_x(entities, entity, angle, velocity);
	move_player_y(entities, entity, angle, velocity);
}

static void	walks(t_list *entities, t_entity *entity, double delta_time)
{
	if (entity->controller.walking_backward)
		player_walk(entities, entity, entity->coords.yaw - 180.0, delta_time);
	if (entity->controller.walking_right)
		player_walk(entities, entity, entity->coords.yaw + 90.0, delta_time);
	if (entity->controller.walking_left)
		player_walk(entities, entity, entity->coords.yaw - 90.0, delta_time);
	if (entity->controller.walking_forward)
		player_walk(entities, entity, entity->coords.yaw, delta_time);
}

static void	mouse_moviment(t_entity *entity, double delta_time)
{
	double	velocity;

	if (!entity->controller.mouse_moviment)
		return ;
	velocity = (entity->controller.mouse_moviment
			* entity->controller.mouse_look_velocity) * delta_time;
	entity->coords.yaw = ft_normalize_angle(entity->coords.yaw + velocity);
}

static void	actions(t_list *entities, t_character *character)
{
	t_entity	*overlapping_entity;
	t_entity	*target_entity;

	overlapping_entity = position_overlaps(entities, (t_entity *)character,
			character->billboard.entity.coords);
	if (overlapping_entity
		&& overlapping_entity->type == ENTITY_DOOR
		&& !((t_door *)overlapping_entity)->opened
		&& overlapping_entity->action)
		overlapping_entity->action(overlapping_entity, character);
	if (character->billboard.entity.controller.action
		&& character->billboard.entity.controller.already_actioned)
		return ;
	character->billboard.entity.controller.already_actioned
		= character->billboard.entity.controller.action;
	if (!character->billboard.entity.controller.action)
		return ;
	target_entity = character->target_entity;
	if (!target_entity)
		return ;
	if (target_entity->action)
		target_entity->action((t_entity *)target_entity, character);
}

void	moviment_frame(t_game *game, t_entity *entity, double delta_time)
{
	if (!entity->health)
		return ;
	if (entity->character)
		actions(game->entities, (t_character *)entity);
	looks(entity, delta_time);
	mouse_moviment(entity, delta_time);
	walks(game->entities, entity, delta_time);
}
