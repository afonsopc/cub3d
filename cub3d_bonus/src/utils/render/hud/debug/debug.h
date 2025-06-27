/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:04:15 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/18 20:51:47 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "../hud.h"

int		render_debug_str(t_game *game, t_ftm_image *canvas, char *str,
			t_coords coords);

void	render_debug_fps(t_game *game, t_ftm_image *canvas, t_coords coords);
void	render_debug_target(t_game *game, t_ftm_image *canvas,
			t_coords coords, t_character *character);
void	render_debug_player(t_game *game, t_ftm_image *canvas,
			t_coords coords, t_character *character);
void	render_debug_entities(t_game *game, t_ftm_image *canvas,
			t_coords coords);

#endif