/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:04:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/18 20:44:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	render_debug_fps(t_game *game, t_ftm_image *canvas, t_coords coords)
{
	char	*fps;
	char	*fps_min;
	char	*fps_max;
	char	*fps_limit;

	fps = ft_strf("%d", game->fps.fps);
	fps_min = ft_strf("Min: %d", game->fps.min);
	fps_max = ft_strf("Max: %d", game->fps.max);
	fps_limit = ft_strf("Limit: %d", game->fps.fps_limit);
	coords.x += render_debug_str(game, canvas, "FPS", coords) + 5;
	coords.x += render_debug_str(game, canvas, fps, coords) + 5;
	coords.x += render_debug_str(game, canvas, fps_min, coords) + 5;
	coords.x += render_debug_str(game, canvas, fps_max, coords) + 5;
	render_debug_str(game, canvas, fps_limit, coords);
	(free(fps), free(fps_min), free(fps_max), free(fps_limit));
}
