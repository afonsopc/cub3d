/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/22 22:56:24 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"

void	entity_frame(t_game *game, t_entity *entity, double delta_time)
{
	(void)game;
	if (entity->controller.frame)
		entity->controller.frame(game, entity, delta_time);
}

void	clear_entity(void *entity)
{
	(void)entity;
}

void	entity_action(t_entity *entity, t_character *actioner)
{
	((void)entity, (void)actioner);
}

void	entity_shot(t_entity *shooted, t_character *shooter)
{
	t_weapon	*weapon;

	if (!shooted || !shooter)
		return ;
	weapon = (t_weapon *)shooter->inventory[shooter->inventory_index];
	if (!weapon || !weapon->item.weapon || shooted->invencible)
		return ;
	shooted->health -= weapon->damage;
	if (shooted->health > shooted->max_health)
		shooted->health = shooted->max_health;
	if (shooted->health < 0)
		shooted->health = 0;
}
