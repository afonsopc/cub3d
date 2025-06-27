/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:26:35 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:53:46 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void	init_player_controller(t_entity *entity)
{
	entity->controller.frame = frame;
	entity->controller.key_look_velocity = PLAYER_KEY_LOOK_VELOCITY;
	entity->controller.mouse_look_velocity = PLAYER_MOUSE_LOOK_VELOCITY;
	entity->controller.walk_velocity = PLAYER_WALK_VELOCITY;
	entity->controller.sprint_velocity = PLAYER_SPRINT_VELOCITY;
	entity->controller.key = key;
}
