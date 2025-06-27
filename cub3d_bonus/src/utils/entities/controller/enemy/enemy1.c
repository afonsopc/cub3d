/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:39:40 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:52:07 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "enemy.h"

void	look_at_target(t_entity *entity, t_entity *target_entity)
{
	double	angle;

	angle = ft_angle_distance(entity->coords, target_entity->coords);
	entity->controller.prev_target = target_entity;
	entity->controller.last_seen_target = ft_get_time();
	entity->controller.last_target_position = target_entity->coords;
	if (angle >= 180)
		entity->coords.yaw -= angle;
	else
		entity->coords.yaw += angle;
	entity->controller.prev_angle = entity->coords.yaw;
}

void	update_movement(t_entity *entity, t_entity *target_entity)
{
	double	dist;

	dist = ft_distance(entity->coords, target_entity->coords);
	if (dist > entity->controller.optimal_proximity + 1.5)
	{
		entity->controller.sprinting = true;
		entity->controller.walking_forward = true;
	}
	else if (dist < entity->controller.optimal_proximity - 1.5)
		entity->controller.walking_backward = true;
	if (ft_get_time() - entity->controller.last_strafe > STRAFE_INTERVAL)
	{
		entity->controller.last_strafe = ft_get_time();
		if (rand() % 2)
			entity->controller.walking_left = true;
		else
			entity->controller.walking_right = true;
	}
}

void	remove_attributes(t_character *character, t_controller *controller)
{
	t_item	*item;

	item = character->inventory[character->inventory_index];
	controller->walking_forward = false;
	controller->walking_backward = false;
	controller->walking_left = false;
	controller->walking_right = false;
	controller->sprinting = false;
	controller->looking_left = false;
	controller->looking_right = false;
	if (item && ft_get_time() - item->last_use >= item->use_delay)
		item->user = NULL;
	character->target_entity = NULL;
}
