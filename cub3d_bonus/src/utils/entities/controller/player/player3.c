/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:52:50 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:53:02 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void	frame(t_game *game, t_entity *entity, double delta_time)
{
	moviment_frame(game, entity, delta_time);
	if (entity->character)
		targets_frame(game, (t_character *)entity, PLAYER_FOV);
}

void	mouse_inv_keys(t_character *character,
	t_ftm_key_hook_values key_hook_values)
{
	int	new_index;

	new_index = character->inventory_index;
	if (key_hook_values.key == FTM_MOUSE_SCROLL_UP
		&& ft_get_time() - character->last_inventory_scroll
		>= INVENTORY_SCROLL_DELAY)
	{
		new_index = (new_index + 1) % INVENTORY_SIZE;
		character->last_inventory_scroll = ft_get_time();
	}
	if (key_hook_values.key == FTM_MOUSE_SCROLL_DOWN
		&& ft_get_time() - character->last_inventory_scroll
		>= INVENTORY_SCROLL_DELAY)
	{
		new_index = (new_index - 1 + INVENTORY_SIZE) % INVENTORY_SIZE;
		character->last_inventory_scroll = ft_get_time();
	}
	if (!character->inventory[new_index])
		return ;
	character->inventory_index = new_index;
}

void	do_keys(t_game *game, t_entity *entity, t_player_keys keys,
	t_ftm_key_hook_values khv)
{
	do_internal_keys(entity, khv);
	do_half_of_keys(&entity->controller, keys, khv);
	do_inv_keys(game, entity, keys, khv);
}

void	key(t_game *game, t_entity *entity, t_ftm_key_hook_values khv)
{
	int	player_id;

	if (!entity || !entity->active || entity->type != ENTITY_PLAYER)
		return ;
	player_id = get_player_id_with_keyboard_only_accounted_for(
			game, (t_player *)entity);
	if (!entity->controller.keyboard_only && khv.controller
		&& khv.controller->id == player_id)
		return (do_keys(game, entity, get_player_gamepad_keys(), khv));
	if (entity == (t_entity *)game->players[0])
		do_keys(game, entity, get_player_keyboard_keys(), khv);
}
