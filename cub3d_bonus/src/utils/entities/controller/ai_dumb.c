/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_dumb.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:34:50 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:38:58 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"

#define AI_MOVE_INTERVAL 2.0

static void	do_the_dumb_ai(t_entity *entity, t_player *player)
{
	double			angle_to_player;
	double			dx;
	double			dy;
	double			distance_to_player;

	if (!player || !entity->character)
		return ;
	dx = player->character.billboard.entity.coords.x - entity->coords.x;
	dy = player->character.billboard.entity.coords.y - entity->coords.y;
	angle_to_player = ft_degrees(atan2(dy, dx));
	entity->coords.yaw = ft_normalize_angle(angle_to_player);
	distance_to_player = sqrt(dx * dx + dy * dy);
	entity->controller.walking_forward = (distance_to_player > 2.0);
	entity->controller.walking_left = false;
	entity->controller.walking_right = false;
	entity->controller.walking_backward = false;
	entity->controller.sprinting = (distance_to_player > 4.0);
}

static void	frame(t_game *game, t_entity *entity, double delta_time)
{
	int	i;

	i = -1;
	while (++i < PLAYER_MAX)
		do_the_dumb_ai(entity, game->players[i]);
	moviment_frame(game, entity, delta_time);
}

void	init_ai_dumb_controller(t_entity *entity)
{
	entity->controller.frame = frame;
	entity->controller.key_look_velocity = PLAYER_KEY_LOOK_VELOCITY / 1.5;
	entity->controller.mouse_look_velocity = PLAYER_MOUSE_LOOK_VELOCITY / 1.5;
	entity->controller.walk_velocity = PLAYER_WALK_VELOCITY / 1.5;
	entity->controller.sprint_velocity = PLAYER_SPRINT_VELOCITY / 1.5;
}
