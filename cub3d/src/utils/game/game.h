/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:16:32 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/14 15:07:48 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include <cub3d.h>

void	init_enviroment_e(t_game *game);
void	init_sprites_e(t_ftm_window *window, t_game *game);
void	init_entities_e(t_game *game);

#endif