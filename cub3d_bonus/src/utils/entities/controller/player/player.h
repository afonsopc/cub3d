/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:25:53 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:54:46 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include "../controller.h"

void			item_use_key(bool use, t_character *character);
void			item_drop_key(t_game *game, bool drop, t_character *character);
void			move_inventory_index(t_character *character);
void			set_key_bool_value(bool *key_bool, t_player_key player_key,
					t_ftm_key_hook_values key_hook_values);
void			do_half_of_keys(t_controller *cont, t_player_keys keys,
					t_ftm_key_hook_values khv);

t_player_keys	get_player_keyboard_keys(void);
t_player_keys	get_player_gamepad_keys(void);
void			do_internal_keys(t_entity *entity, t_ftm_key_hook_values khv);
void			do_inv_keys(t_game *game, t_entity *entity, t_player_keys keys,
					t_ftm_key_hook_values khv);
int				get_player_id_with_keyboard_only_accounted_for(
					t_game *game, t_player *player);

void			frame(t_game *game, t_entity *entity, double delta_time);
void			mouse_inv_keys(t_character *character,
					t_ftm_key_hook_values key_hook_values);
void			do_keys(t_game *game, t_entity *entity, t_player_keys keys,
					t_ftm_key_hook_values khv);
void			key(t_game *game, t_entity *entity, t_ftm_key_hook_values khv);

#endif
