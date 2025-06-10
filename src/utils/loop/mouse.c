/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 11:06:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 16:43:46 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loop.h"

void	mouse_hook(t_coords coords)
{
	t_player		*player;
	t_ftm_window	*window;
	double			movement;

	window = &cub3d()->window;
	if (!window->using_mouse)
		return ;
	mlx_mouse_move(window->display, window->win, window->size.width / 2, window->size.height / 2);
	movement = coords.x - (window->size.width / 2);
	if (fabs(movement) < 10)
		movement = movement * (0.5 + (fabs(movement) / 20.0));
	player = cub3d()->game.player;
	if (player)
		player->character.billboard.entity.controller.mouse_moviment = movement;
}
