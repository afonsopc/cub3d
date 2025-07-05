/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/05 12:57:05 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void	player_frame(t_game *game, t_entity *entity, double delta_time)
{
	t_player	*player;
	character_frame(game, entity, delta_time);
	if (!entity)
		return ;
	player = (t_player *)entity;
	entity->actionable = player->character.dead;
	entity->targetable = true;
	if ((!game->players[1] || !((t_entity *)game->players[1])->active)
		&& player->character.dead
		&& ft_get_time() - player->character.died_at
		>= PLAYER_DEAD_RESET_DELAY)
		cub3d()->new_map_path = cub3d()->curr_map->path;
}

void	clear_player(void *data)
{
	t_player	*player;

	clear_character(data);
	if (!data)
		return ;
	player = (t_player *)data;
	ftm_free_image(player->canvas);
}

void	player_action(t_entity *entity, t_character *actioner)
{
	t_player	*player;

	character_action(entity, actioner);
	if (!entity || !actioner)
		return ;
	player = (t_player *)entity;
	if (player->character.dead)
		entity->health = entity->max_health / 2;
}

void	player_shot(t_entity *shooted, t_character *shooter)
{
	if (shooted->type == ENTITY_PLAYER
		&& shooter->billboard.entity.type == ENTITY_PLAYER
		&& !((t_player *)shooted)->friendly_fire)
		return ;
	character_shot(shooted, shooter);
}
