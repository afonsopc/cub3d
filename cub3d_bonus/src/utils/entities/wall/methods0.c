/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 00:56:01 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wall.h"

void	wall_frame(t_game *game, t_entity *entity, double delta_time)
{
	entity_frame(game, entity, delta_time);
}

void	clear_wall(void *data)
{
	t_wall	*wall;

	clear_entity(data);
	if (!data)
		return ;
	wall = (t_wall *)data;
	free(wall->north_sprite);
	free(wall->south_sprite);
	free(wall->east_sprite);
	free(wall->west_sprite);
}

void	wall_action(t_entity *entity, t_character *actioner)
{
	entity_action(entity, actioner);
}

void	wall_shot(t_entity *shooted, t_character *shooter)
{
	entity_shot(shooted, shooter);
}
