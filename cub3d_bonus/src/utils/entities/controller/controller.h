/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:31:30 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:30:31 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROLLER_H
# define CONTROLLER_H

# include "../entities.h"

typedef struct s_player_key
{
	int			key;
	bool		use_pressure;
	double		min_pressure;
	double		max_pressure;
	bool		use_coords;
	t_coords	min_coords;
	t_coords	max_coords;
}	t_player_key;

typedef struct s_player_keys
{
	t_player_key	walking_forward;
	t_player_key	walking_backward;
	t_player_key	walking_left;
	t_player_key	walking_right;
	t_player_key	looking_left;
	t_player_key	looking_right;
	t_player_key	action;
	t_player_key	sprinting;
	t_player_key	move_inventory_index;
	t_player_key	item_use;
	t_player_key	item_drop;
	t_player_key	keyboard_only;
	t_player_key	activate;
}	t_player_keys;

void	init_player_controller(t_entity *entity);
void	init_ai_dumb_controller(t_entity *entity);
void	init_enemy_controller(t_game *game, t_entity *entity);
void	moviment_frame(t_game *game, t_entity *entity, double delta_time);
void	targets_frame(t_game *game, t_character *character, double fov);

#endif
