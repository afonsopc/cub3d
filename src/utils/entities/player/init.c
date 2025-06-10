/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 16:59:26 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void	init_player(t_game *game, t_ftm_window *window, t_player *player, char identifier)
{
	if (!hashmap_get_with_identifier(game, game->map->types, identifier, "CONTROLLER"))
		ft_hashmap_set(game->map->types, "CONTROLLER", "PLAYER", NULL);
	init_character(game, window, (t_character *)player, identifier);
	player->character.billboard.entity.type = ENTITY_PLAYER;
	player->character.billboard.entity.frame = player_frame;
	player->character.billboard.entity.clear = clear_player;
	player->character.billboard.entity.action = player_action;
	player->character.billboard.entity.action = player_shot;
	if (identifier == 'N')
		player->character.billboard.entity.coords.yaw = 270.0;
	else if (identifier == 'S')
		player->character.billboard.entity.coords.yaw = 90.0;
	else if (identifier == 'W')
		player->character.billboard.entity.coords.yaw = 180.0;
	player->character.billboard.entity.size = (t_dsize){PLAYER_WIDTH, PLAYER_HEIGHT};
	player->character.billboard.entity.collision_sound = hashmap_get_with_identifier(game, game->sounds, identifier, "COLLISION");
}

t_player	*player_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_player	*player;

	player = ft_calloc(1, sizeof(t_player));
	if (!player)
		return (NULL);
	init_player(game, window, player, identifier);
	return (player);
}
