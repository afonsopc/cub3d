/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:10:00 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 16:58:07 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	add_player_e(t_game *game, t_player *player)
{
	int	i;

	fte_set(NULL);
	i = -1;
	while (++i < PLAYER_MAX)
	{
		if (game->players[i])
			continue ;
		player->character.billboard.entity.active = false;
		game->players[i] = player;
		return ;
	}
	fte_set("Too many players");
}

static void	create_entity_e(t_game *game, t_ftm_window *window, char c,
	t_size position)
{
	t_type_creator		creator;
	t_entity			*entity;

	fte_set(NULL);
	entity = NULL;
	creator = get_type_creator(game->map->identifiers, c);
	if (fte_flagged() || !creator)
		return ;
	entity = creator(game, window, c);
	if (fte_flagged())
		return ;
	if (!entity)
		return (fte_set("entity creation"));
	entity->identifier = c;
	entity->coords.x = position.width;
	entity->coords.y = position.height;
	if (entity->billboard)
	{
		entity->coords.x += 0.5 - entity->size.width * 0.5;
		entity->coords.y += 0.5 - entity->size.height * 0.5;
	}
	ft_list_add(&game->entities, entity, free_entity);
	if (entity->type == ENTITY_PLAYER)
		add_player_e(game, (t_player *)entity);
}

void	init_entities_e(t_game *game, t_ftm_window *window)
{
	t_size	s;

	s.height = -1;
	while (game->map->map[++s.height])
	{
		s.width = -1;
		while (game->map->map[s.height][++s.width])
		{
			if (create_entity_e(game, window, game->map->map[s.height][s.width],
				s), fte_flagged())
				return ;
			if (fte_flagged())
				return ;
		}
	}
}
