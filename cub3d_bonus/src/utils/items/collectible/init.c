/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 20:50:24 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collectible.h"

void	init_collectible(t_game *game, t_ftm_window *window,
	t_collectible *collectible, char identifier)
{
	if (!collectible)
		return ;
	init_item(game, window, (t_item *)collectible, identifier);
	collectible->item.collectible = true;
	collectible->item.clear = clear_collectible;
	collectible->item.frame = collectible_frame;
	collectible->item.use = collectible_use;
	collectible->item.drop = collectible_drop;
	collectible->health = fabs(ft_atof(hashmap_get_with_identifier(game,
					game->map->types, identifier, "HEALTH")));
	collectible->ammo = fabs(ft_atof(hashmap_get_with_identifier(game,
					game->map->types, identifier, "AMMO")));
	collectible->score = fabs(ft_atof(hashmap_get_with_identifier(game,
					game->map->types, identifier, "SCORE")));
}

t_collectible	*collectible_new(t_game *game, t_ftm_window *window,
	char identifier)
{
	t_collectible	*collectible;

	collectible = ft_calloc(1, sizeof(t_collectible));
	if (!collectible)
		return (NULL);
	init_collectible(game, window, collectible, identifier);
	return (collectible);
}
