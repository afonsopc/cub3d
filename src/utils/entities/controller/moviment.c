/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moviment.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:51:55 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 21:48:28 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"

static void	looks(t_entity *entity, double delta_time)
{
	double	velocity;

	velocity = entity->controller.key_look_velocity * delta_time;
	if (entity->controller.looking_left)
		entity->coords.yaw = ft_normalize_angle(entity->coords.yaw - velocity);
	else if (entity->controller.looking_right)
		entity->coords.yaw = ft_normalize_angle(entity->coords.yaw + velocity);
}

static t_entity	*position_overlaps(t_list *entities, t_entity *entity, t_coords coords)
{
	t_list		*curr;
	t_entity	*curr_entity;

	if (!entity->hard)
		return (false);
	curr = entities;
	while (curr)
	{
		curr_entity = curr->data;
        if (curr_entity != entity && curr_entity->hard
            && coords.x + entity->size.width >= curr_entity->coords.x
            && coords.x - entity->size.width <= (curr_entity->coords.x + curr_entity->size.width)
            && coords.y + entity->size.height >= curr_entity->coords.y
            && coords.y - entity->size.height <= (curr_entity->coords.y + curr_entity->size.height))
			return (fta_play(entity->collision_sound), curr_entity);
		curr = curr->next;
	}
	return (NULL);
}

static void	move_player_x(t_list *entities, t_entity *entity, double angle, double velocity)
{
	double	new_x;

	new_x = entity->coords.x + velocity * ft_cos_degrees(angle);
	if (!position_overlaps(entities, entity, (t_coords){new_x, entity->coords.y, 0}))
		entity->coords.x = new_x;
}

static void	move_player_y(t_list *entities, t_entity *entity, double angle, double velocity)
{
	double	new_y;

	new_y = entity->coords.y + velocity * ft_sin_degrees(angle);
	if (!position_overlaps(entities, entity, (t_coords){entity->coords.x, new_y, 0}))
		entity->coords.y = new_y;
}

static void	player_walk(t_list *entities, t_entity *entity, double angle, double delta_time)
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
	velocity = (entity->controller.mouse_moviment * entity->controller.mouse_look_velocity) * delta_time;
	entity->coords.yaw = ft_normalize_angle(entity->coords.yaw + velocity);
}

static void	actions(t_list *entities, t_character *character)
{
	t_entity	*overlapping_entity;
	t_entity	*target_entity;

	overlapping_entity = position_overlaps(entities, (t_entity *)character, character->billboard.entity.coords);
	if (overlapping_entity
		&& overlapping_entity->type == ENTITY_DOOR
		&& !((t_door *)overlapping_entity)->opened
		&& overlapping_entity->action)
		overlapping_entity->action(overlapping_entity, character);
	if (character->billboard.entity.controller.action && character->billboard.entity.controller.already_actioned)
		return ;
	character->billboard.entity.controller.already_actioned = character->billboard.entity.controller.action;
	if (!character->billboard.entity.controller.action)	
		return ;
	target_entity = character->target_entity;
	if (!target_entity)
		return ;
	if (target_entity->action)
		target_entity->action((t_entity *)target_entity, character);
}

void	moviment_frame(t_entity *entity, double delta_time)
{
	if (entity->character)
		actions(cub3d()->game.entities, (t_character *)entity);
	looks(entity, delta_time);
	mouse_moviment(entity, delta_time);
	walks(cub3d()->game.entities, entity, delta_time);
}
