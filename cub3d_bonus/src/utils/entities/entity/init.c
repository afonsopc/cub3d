/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 00:58:45 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"

void	do_amazing_things(t_game *game, t_entity *entity, char identifier)
{
	entity->targetable = ft_strequal(
			hashmap_get_with_identifier(game, game->map->types, identifier,
				"TARGETABLE"), "TRUE");
	entity->ultra_mega_transparent = ft_strequal(
			hashmap_get_with_identifier(game, game->map->types, identifier,
				"ULTRA_MEGA_TRANSPARENT"), "TRUE");
	entity->transparent = ft_strequal(
			hashmap_get_with_identifier(game, game->map->types, identifier,
				"TRANSPARENT"), "TRUE");
	entity->no_transparency_for_bill = ft_strequal(
			hashmap_get_with_identifier(game, game->map->types, identifier,
				"NO_TRANSPARENCY_FOR_BILL"), "TRUE");
	entity->active = true;
	entity->hard = !ft_strequal(
			hashmap_get_with_identifier(game, game->map->types, identifier,
				"HARD"), "FALSE");
	entity->collision_sound = hashmap_get_with_identifier(game, game->sounds,
			identifier, "COLLISION");
	entity->identifier = identifier;
	entity->size = (t_dsize){1, 1};
	init_controller(game, entity,
		hashmap_get_with_identifier(game, game->map->types, identifier,
			"CONTROLLER"));
}

void	init_entity(t_game *game, t_ftm_window *window, t_entity *entity,
	char identifier)
{
	(void)window;
	if (!entity)
		return ;
	entity->type = ENTITY;
	entity->frame = entity_frame;
	entity->clear = clear_entity;
	entity->action = entity_action;
	entity->shot = entity_shot;
	entity->max_health = ft_atoi(hashmap_get_with_identifier(game,
				game->map->types, identifier, "MAX_HEALTH"));
	if (!entity->max_health)
		entity->max_health = CHARACTER_DEFAULT_MAX_HEALTH;
	entity->health = entity->max_health;
	do_amazing_things(game, entity, identifier);
}

t_entity	*entity_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_entity	*entity;

	entity = ft_calloc(1, sizeof(t_entity));
	if (!entity)
		return (NULL);
	init_entity(game, window, entity, identifier);
	return (entity);
}
