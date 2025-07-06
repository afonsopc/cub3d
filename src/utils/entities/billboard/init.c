/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/06 22:34:26 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "billboard.h"

void	init_billboard(t_game *game, t_ftm_window *window,
	t_billboard *billboard, char identifier)
{
	init_entity(game, window, (t_entity *)billboard, identifier);
	billboard->entity.type = ENTITY_BILLBOARD;
	billboard->entity.frame = billboard_frame;
	billboard->entity.clear = clear_billboard;
	billboard->entity.action = billboard_action;
	billboard->entity.shot = billboard_shot;
	billboard->entity.billboard = true;
	billboard->entity.size = (t_dsize){BILLBOARD_WIDTH, BILLBOARD_HEIGHT};
	billboard->sprites = ft_calloc(360, sizeof(t_sprite *));
	billboard->y_centered = ft_strequal(
		hashmap_get_with_identifier(game, game->map->types, identifier,
			"Y_CENTERED"), "TRUE");
	fill_3d_sprites_from_game(game, billboard->sprites, identifier, "STILL");
}

t_billboard	*billboard_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_billboard	*billboard;

	billboard = ft_calloc(1, sizeof(t_billboard));
	if (!billboard)
		return (NULL);
	init_billboard(game, window, billboard, identifier);
	return (billboard);
}
