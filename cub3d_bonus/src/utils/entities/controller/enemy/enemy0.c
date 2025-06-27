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

#include "enemy.h"

static void	do_has_target(t_entity *entity, t_character *character)
{
	look_at_target(entity, character->target_entity);
	if (character->inventory_index == 0
		&& character->inventory[1])
		character->inventory[0]->user = character;
	if (character->inventory[character->inventory_index]
		&& (double)rand() / RAND_MAX < FIRE_CHANCE)
		shoot(character);
	else if (!character->inventory[character->inventory_index]
		|| !character->inventory[character->inventory_index]->user)
		update_movement(entity, character->target_entity);
}

static void	frame(t_game *game, t_entity *entity, double delta_time)
{
	t_character	*character;

	if (!entity->character)
		return ;
	character = (t_character *)entity;
	remove_attributes(character, &entity->controller);
	if (character->dead)
		return ;
	targets_frame(game, character, 120);
	if (ft_get_time() - character->last_hit < LAST_HIT_DELAY
		&& character->last_hit_by_character)
		character->target_entity = (t_entity *)character->last_hit_by_character;
	hearment(game, character);
	if (!character->target_entity || character->target_entity->type
		!= ENTITY_PLAYER
		|| ((t_character *)character->target_entity)->dead)
		look_around(character, &entity->controller);
	else
		do_has_target(entity, character);
	moviment_frame(game, entity, delta_time);
}

void	init_enemy_controller(t_game *game, t_entity *entity)
{
	entity->controller.optimal_proximity = ft_atof(hashmap_get_with_identifier(
				game, game->map->types, entity->identifier,
				"OPTIMAL_PROXIMITY"));
	entity->controller.frame = frame;
	entity->controller.prev_angle = entity->coords.yaw;
	entity->controller.key_look_velocity = PLAYER_KEY_LOOK_VELOCITY / 1.5;
	entity->controller.mouse_look_velocity = PLAYER_MOUSE_LOOK_VELOCITY / 1.5;
	entity->controller.walk_velocity = PLAYER_WALK_VELOCITY / 1.5;
	entity->controller.sprint_velocity = PLAYER_SPRINT_VELOCITY / 1.5;
}
