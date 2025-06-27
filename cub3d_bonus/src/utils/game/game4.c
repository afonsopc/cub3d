/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:43:09 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/04 01:22:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	kill_threads(t_game *game)
{
	int	i;

	i = -1;
	while (++i < CAMERA_THREADS)
		ftt_free_thread(game->camera_threads[i]);
}

void	init_threads_e(t_game *game)
{
	int	i;

	fte_set(NULL);
	i = -1;
	while (++i < CAMERA_THREADS)
	{
		game->camera_threads[i] = ftt_thread_new();
		if (!game->camera_threads[i])
			return (fte_set("thread init"));
	}
}

void	init_fps(t_game *game)
{
	game->fps.fps_limit = FPS_LIMIT;
	game->fps.beginning = ft_get_time();
	game->fps.last_frame_time = ft_get_time();
	game->fps.delta_time = DELTA_TIME_START;
}
