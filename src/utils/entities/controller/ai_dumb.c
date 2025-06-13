/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_dumb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:34:50 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/12 19:56:57 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"

#define AI_MOVE_INTERVAL 2.0
static void	frame(t_entity *entity, double delta_time)
{
    double			angle_to_player;
    double			dx;
	double			dy;
	double			distance_to_player;
	t_character		*character;

    if (!cub3d()->game.player || !entity->character)
		return ;
	character = (t_character *)entity;
	dx = cub3d()->game.player->character.billboard.entity.coords.x - entity->coords.x;
	dy = cub3d()->game.player->character.billboard.entity.coords.y - entity->coords.y;
	angle_to_player = ft_degrees(atan2(dy, dx));
	entity->coords.yaw = ft_normalize_angle(angle_to_player);
	distance_to_player = sqrt(dx * dx + dy * dy);
	entity->controller.walking_forward = (distance_to_player > 2.0);
	entity->controller.walking_left = false;
	entity->controller.walking_right = false;
	entity->controller.walking_backward = false;
	entity->controller.sprinting = (distance_to_player > 4.0);
	if (character->inventory[character->inventory_index])
	{
		if (rand() % 1000 == 0)
			character->inventory[character->inventory_index]->user = character;
		else
			character->inventory[character->inventory_index]->user = NULL;
	}
    moviment_frame(entity, delta_time);
}

void	init_ai_dumb_controller(t_entity *entity)
{
	entity->controller.frame = frame;
	entity->controller.key_look_velocity = PLAYER_KEY_LOOK_VELOCITY / 1.5;
	entity->controller.mouse_look_velocity = PLAYER_MOUSE_LOOK_VELOCITY / 1.5;
	entity->controller.walk_velocity = PLAYER_WALK_VELOCITY / 1.5;
	entity->controller.sprint_velocity = PLAYER_SPRINT_VELOCITY / 1.5;
}
