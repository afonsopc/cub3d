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

#include "food.h"

void	init_food(t_game *game, t_ftm_window *window, t_food *food,
	char identifier)
{
	if (!food)
		return ;
	init_item(game, window, (t_item *)food, identifier);
	food->item.food = true;
	food->item.clear = clear_food;
	food->item.frame = food_frame;
	food->item.use = food_use;
	food->health = fabs(ft_atof(hashmap_get_with_identifier(game, game->map->types,
			identifier, "HEALTH")));
}

t_food	*food_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_food	*food;

	food = ft_calloc(1, sizeof(t_food));
	if (!food)
		return (NULL);
	init_food(game, window, food, identifier);
	return (food);
}
