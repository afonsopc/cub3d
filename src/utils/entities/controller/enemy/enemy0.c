/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:39:40 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:50:32 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "brain.h"

void	init_enemy_controller(t_game *game, t_entity *entity)
{
	entity->controller.optimal_proximity = ft_atof(hashmap_get_with_identifier(
				game, game->map->types, entity->identifier,
				"OPTIMAL_PROXIMITY"));
	entity->controller.frame = enemy_brain_frame;
	entity->controller.prev_angle = entity->coords.yaw;
	entity->controller.key_look_velocity = PLAYER_KEY_LOOK_VELOCITY / 1.5;
	entity->controller.mouse_look_velocity = PLAYER_MOUSE_LOOK_VELOCITY / 1.5;
	entity->controller.walk_velocity = PLAYER_WALK_VELOCITY / 1.5;
	entity->controller.sprint_velocity = PLAYER_SPRINT_VELOCITY / 1.5;
}
