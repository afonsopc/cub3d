/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:16:32 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/21 00:30:39 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include <cub3d.h>

void	init_enviroment_e(t_game *game);
void	init_minimap(t_ftm_window *window, t_game *game);
void	init_sprites_e(t_ftm_window *window, t_game *game);
void	init_window(t_ftm_window *window, t_game *game);
void	init_font_e(t_ftm_window *window, t_game *game);
void	init_entities_e(t_game *game, t_ftm_window *window);
void	kill_threads(t_game *game);
void	init_threads_e(t_game *game);
void	init_sounds_e(t_game *game);
void	init_hud(t_game *game);
void	init_fps(t_game *game);
void	set_sprite_configs(t_game *game);
void	init_fonts_e(t_ftm_window *window, t_game *game);
void	insert_door_frames(t_game *game);
void	init_sprites_3d(t_game *game);

#endif