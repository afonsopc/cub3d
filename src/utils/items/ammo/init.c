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

#include "ammo.h"

void	init_ammo(t_game *game, t_ftm_window *window, t_ammo *ammo,
	char identifier)
{
	if (!ammo)
		return ;
	init_item(game, window, (t_item *)ammo, identifier);
	ammo->item.clear = clear_ammo;
	ammo->item.frame = ammo_frame;
	ammo->item.use = ammo_use;
	ammo->amount = abs(ft_atoi(hashmap_get_with_identifier(game, game->map->types,
			identifier, "AMOUNT")));
}

t_ammo	*ammo_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_ammo	*ammo;

	ammo = ft_calloc(1, sizeof(t_ammo));
	if (!ammo)
		return (NULL);
	init_ammo(game, window, ammo, identifier);
	return (ammo);
}
