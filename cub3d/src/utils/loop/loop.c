/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:20:20 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/21 18:24:44 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loop.h"

static double	get_delta_time(void)
{
	static struct timeval	last_time = {0, 0};
	struct timeval			current_time;
	double					delta;

	if (last_time.tv_sec == 0 && last_time.tv_usec == 0)
	{
		gettimeofday(&last_time, NULL);
		return (FPS60);
	}
	gettimeofday(&current_time, NULL);
	delta = (current_time.tv_sec - last_time.tv_sec) + (current_time.tv_usec
			- last_time.tv_usec) / 1000000.0;
	last_time = current_time;
	return (delta);
}

void	loop(void)
{
	call_entity_frames(cub3d()->game.entities, get_delta_time());
	render_game(&cub3d()->window, &cub3d()->game);
}
