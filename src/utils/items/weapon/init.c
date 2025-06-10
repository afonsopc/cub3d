/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 16:50:19 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "weapon.h"

void	init_weapon(t_game *game, t_ftm_window *window, t_weapon *weapon,
	char identifier)
{
	if (!weapon)
		return ;
	init_item(game, window, (t_item *)weapon, identifier);
	weapon->item.weapon = true;
	weapon->item.clear = clear_weapon;
	weapon->item.frame = weapon_frame;
	weapon->item.use = weapon_use;
	weapon->damage = ft_atoi(hashmap_get_with_identifier(game, game->map->types, identifier, "DAMAGE"));
	weapon->ammo_usage = ft_atoi(hashmap_get_with_identifier(game, game->map->types, identifier, "AMMO_USAGE"));
}

t_weapon	*weapon_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_weapon	*weapon;

	weapon = ft_calloc(1, sizeof(t_weapon));
	if (!weapon)
		return (NULL);
	init_weapon(game, window, weapon, identifier);
	return (weapon);
}
