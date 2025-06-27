/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 19:04:26 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "elevator.h"

void	init_elevator(t_game *game, t_ftm_window *window,
	t_elevator *elevator, char identifier)
{
	init_wall(game, window, (t_wall *)elevator, identifier);
	elevator->wall.entity.type = ENTITY_ELEVATOR;
	elevator->wall.entity.frame = elevator_frame;
	elevator->wall.entity.clear = clear_elevator;
	elevator->wall.entity.action = elevator_action;
	elevator->wall.entity.shot = elevator_shot;
	elevator->map_path = hashmap_get_with_identifier(game, game->map->types,
			identifier, "MAP_PATH");
	elevator->wall.entity.actionable = !!elevator->map_path;
}

t_elevator	*elevator_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_elevator	*elevator;

	fte_set(NULL);
	elevator = ft_calloc(1, sizeof(t_elevator));
	if (!elevator)
		return (NULL);
	init_elevator(game, window, elevator, identifier);
	return (elevator);
}
