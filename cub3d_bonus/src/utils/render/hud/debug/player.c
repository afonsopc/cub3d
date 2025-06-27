/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 01:49:57 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/18 21:18:50 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	render_debug_player(t_game *game, t_ftm_image *canvas,
	t_coords coords, t_character *character)
{
	char	*player_x;
	char	*player_y;
	char	*player_yaw;
	char	*player_fov;
	char	*player_health;

	player_x = ft_strf("X: %d",
			(int)character->billboard.entity.coords.x);
	player_y = ft_strf("Y: %d",
			(int)character->billboard.entity.coords.y);
	player_yaw = ft_strf("Yaw: %d",
			(int)character->billboard.entity.coords.yaw);
	player_fov = ft_strf("FOV: %d", (int)character->fov);
	player_health = ft_strf("Health: %d",
			character->billboard.entity.health);
	coords.x += render_debug_str(game, canvas, "Player", coords) + 5;
	coords.x += render_debug_str(game, canvas, player_x, coords) + 5;
	coords.x += render_debug_str(game, canvas, player_y, coords) + 5;
	coords.x += render_debug_str(game, canvas, player_yaw, coords) + 5;
	coords.x += render_debug_str(game, canvas, player_fov, coords) + 5;
	render_debug_str(game, canvas, player_health, coords);
	(free(player_x), free(player_y), free(player_yaw),
		free(player_fov), free(player_health));
}
