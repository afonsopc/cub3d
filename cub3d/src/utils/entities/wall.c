/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/22 11:28:17 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"

static void	wall_frame(t_entity *entity, double delta_time)
{
	(void)entity;
	(void)delta_time;
}

static void	free_wall(void *wall)
{
	free(wall);
}

static void	*hashmap_get_with_identifier(t_hashmap *hashmap, char identifier,
		char *rest)
{
	char	*key;
	void	*data;

	key = ft_strdup(rest);
	if (identifier != '1')
	{
		free(key);
		key = ft_strf("%c_%s", identifier, rest);
	}
	data = ft_hashmap_get_value(hashmap, key);
	return (free(key), data);
}

t_wall	*wall_new(char identifier, t_game *game)
{
	t_wall	*wall;

	wall = ft_calloc(1, sizeof(t_wall));
	if (!wall)
		return (NULL);
	wall->base.type = ENTITY_WALL;
	wall->base.frame = wall_frame;
	wall->base.free = free_wall;
	wall->base.block = true;
	wall->north_sprite = hashmap_get_with_identifier(game->sprites, identifier,
			"NO");
	wall->south_sprite = hashmap_get_with_identifier(game->sprites, identifier,
			"SO");
	wall->west_sprite = hashmap_get_with_identifier(game->sprites, identifier,
			"WE");
	wall->east_sprite = hashmap_get_with_identifier(game->sprites, identifier,
			"EA");
	return (wall);
}
