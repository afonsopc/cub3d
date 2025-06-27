/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:27:20 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:29:06 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

t_player_keys	get_player_keyboard_keys(void)
{
	return ((t_player_keys){
		.walking_forward = {XK_w, false, 0.0, 1.0, false, {0}, {0}},
		.walking_backward = {XK_s, false, 0.0, 1.0, false, {0}, {0}},
		.walking_left = {XK_a, false, 0.0, 1.0, false, {0}, {0}},
		.walking_right = {XK_d, false, 0.0, 1.0, false, {0}, {0}},
		.looking_left = {XK_Left, false, 0.0, 1.0, false, {0}, {0}},
		.looking_right = {XK_Right, false, 0.0, 1.0, false, {0}, {0}},
		.action = {XK_e, false, 0.0, 1.0, false, {0}, {0}},
		.sprinting = {XK_Shift_L, false, 0.0, 1.0, false, {0}, {0}},
		.move_inventory_index = {XK_r, false, 0.0, 1.0, false, {0}, {0}},
		.item_use = {XK_space, false, 0.0, 1.0, false, {0}, {0}},
		.item_drop = {XK_q, false, 0.0, 1.0, false, {0}, {0}},
		.activate = {0},
	});
}

t_player_keys	get_player_gamepad_keys(void)
{
	return ((t_player_keys){
		.walking_forward = {FTM_GAMEPAD_LSTICK, false, 0.0, 1.0,
			true, {0.0, 0.0, 0}, {1.0, 0.3, 0}},
		.walking_backward = {FTM_GAMEPAD_LSTICK, false, 0.0, 1.0,
			true, {0.0, 0.7, 0}, {1.0, 1.0, 0}},
		.walking_left = {FTM_GAMEPAD_LSTICK, false, 0.0, 1.0,
			true, {0.0, 0.0, 0}, {0.3, 1.0, 0}},
		.walking_right = {FTM_GAMEPAD_LSTICK, false, 0.0, 1.0,
			true, {0.7, 0.0, 0}, {1.0, 1.0, 0}},
		.looking_left = {FTM_GAMEPAD_RSTICK, false, 0.0, 1.0,
			true, {0.0, 0.0, 0}, {0.3, 1.0, 0}},
		.looking_right = {FTM_GAMEPAD_RSTICK, false, 0.0, 1.0,
			true, {0.7, 0.0, 0}, {1.0, 1.0, 0}},
		.action = {FTM_GAMEPAD_X, false, 0.0, 1.0, false, {0}, {0}},
		.sprinting = {FTM_GAMEPAD_A, false, 0.0, 1.0, false, {0}, {0}},
		.move_inventory_index = {FTM_GAMEPAD_R1, false, 0.0, 1.0,
			false, {0}, {0}},
		.item_use = {FTM_GAMEPAD_R2, true, 0.8, 1.0, false, {0}, {0}},
		.item_drop = {FTM_GAMEPAD_L2, true, 0.8, 1.0, false, {0}, {0}},
		.activate = {FTM_GAMEPAD_MENU, false, 0.0, 1.0, false, {0}, {0}},
	});
}

void	do_internal_keys(t_entity *entity, t_ftm_key_hook_values khv)
{
	t_character	*character;

	character = (t_character *)entity;
	if (khv.key == XK_i && khv.down)
	{
		character->cheating = true;
		entity->invencible = !entity->invencible;
		entity->health = entity->max_health;
	}
	if (khv.key == XK_b && khv.down)
	{
		character->cheating = true;
		entity->hard = !entity->hard;
	}
}

void	do_inv_keys(t_game *game, t_entity *entity, t_player_keys keys,
	t_ftm_key_hook_values khv)
{
	bool	boolean;

	boolean = false;
	set_key_bool_value(&boolean, keys.move_inventory_index, khv);
	if (boolean)
		move_inventory_index((t_character *)entity);
	set_key_bool_value(&boolean, keys.item_use, khv);
	if (khv.key == keys.item_use.key)
		item_use_key(boolean, (t_character *)entity);
	set_key_bool_value(&boolean, keys.item_drop, khv);
	if (khv.key == keys.item_drop.key)
		item_drop_key(game, boolean, (t_character *)entity);
	if ((t_entity *)game->players[0] == entity)
		mouse_inv_keys((t_character *)entity, khv);
}

int	get_player_id_with_keyboard_only_accounted_for(
	t_game *game, t_player *player)
{
	int	i;

	i = -1;
	while (++i < PLAYER_MAX)
		if (game->players[i] == player)
			break ;
	if (game->players[0]
		&& ((t_entity *)game->players[0])->controller.keyboard_only)
		--i;
	return (i);
}
