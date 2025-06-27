/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:06:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:06:34 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loop.h"

void	mouse_hook(t_coords coords)
{
	t_player		*player;
	t_ftm_window	*window;
	double			movement;
	t_game			*game;

	window = cub3d()->window;
	if (!window->using_mouse)
		return ;
	game = cub3d()->game;
	pthread_mutex_lock(&cub3d()->game_mutex);
	if (game != cub3d()->game || !game)
	{
		pthread_mutex_unlock(&cub3d()->game_mutex);
		return ;
	}
	mlx_mouse_move(window->display, window->win, window->size.width / 2,
		window->size.height / 2);
	movement = coords.x - (window->size.width / 2);
	if (fabs(movement) < 10)
		movement = movement * (0.5 + (fabs(movement) / 20.0));
	player = cub3d()->game->players[0];
	if (player)
		player->character.billboard.entity.controller.mouse_moviment = movement;
	pthread_mutex_unlock(&cub3d()->game_mutex);
}
