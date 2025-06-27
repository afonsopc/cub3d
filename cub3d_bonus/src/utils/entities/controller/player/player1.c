/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:26:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:06:07 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void	item_use_key(bool use, t_character *character)
{
	t_item	*item;

	item = character->inventory[character->inventory_index];
	if (!item)
		return ;
	item->user = NULL;
	if (use)
		item->user = character;
}

void	item_drop_key(t_game *game, bool drop, t_character *character)
{
	t_item	*item;

	item = character->inventory[character->inventory_index];
	if (!item)
		return ;
	if (drop)
		item->drop(game, cub3d()->window, item, character);
}

void	move_inventory_index(t_character *character)
{
	int	i;

	i = character->inventory_index;
	while (true)
	{
		i += 1;
		if (i < 0)
			i = INVENTORY_SIZE - 1;
		if (i >= INVENTORY_SIZE)
			i = 0;
		if (character->inventory[i] || i == character->inventory_index)
		{
			character->inventory_index = i;
			return ;
		}
	}
}

void	set_key_bool_value(bool *key_bool, t_player_key player_key,
	t_ftm_key_hook_values key_hook_values)
{
	bool	from_pressure;
	bool	from_coords;

	if (key_hook_values.key != player_key.key)
		return ;
	if (!player_key.use_coords && !player_key.use_pressure)
	{
		*key_bool = key_hook_values.down;
		return ;
	}
	from_pressure = true;
	from_coords = true;
	if (player_key.use_pressure)
		from_pressure = key_hook_values.pressure
			>= player_key.min_pressure && key_hook_values.pressure
			<= player_key.max_pressure;
	if (player_key.use_coords)
		from_coords = key_hook_values.coords.x >= player_key.min_coords.x
			&& key_hook_values.coords.x <= player_key.max_coords.x
			&& key_hook_values.coords.y >= player_key.min_coords.y
			&& key_hook_values.coords.y <= player_key.max_coords.y;
	*key_bool = from_pressure && from_coords;
}

void	do_half_of_keys(t_controller *cont, t_player_keys keys,
	t_ftm_key_hook_values khv)
{
	set_key_bool_value(&cont->walking_forward, keys.walking_forward, khv);
	set_key_bool_value(&cont->walking_left, keys.walking_left, khv);
	set_key_bool_value(&cont->walking_backward, keys.walking_backward, khv);
	set_key_bool_value(&cont->walking_right, keys.walking_right, khv);
	set_key_bool_value(&cont->looking_right, keys.looking_right, khv);
	set_key_bool_value(&cont->looking_left, keys.looking_left, khv);
	set_key_bool_value(&cont->action, keys.action, khv);
	set_key_bool_value(&cont->sprinting, keys.sprinting, khv);
}
