/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:16:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 13:10:56 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	init_enviroment_e(t_game *game)
{
	char	*ceiling;
	char	*flooring;

	fte_set(ERROR_NO_ERROR);
	if (!game || !game->map || !game->map->types)
		return (fte_set(ERROR_CEILING_OR_FLOORING_UNDEFINED));
	ceiling = ft_hashmap_get_value(game->map->types, "C");
	flooring = ft_hashmap_get_value(game->map->types, "F");
	if (!ceiling || !flooring)
		return (fte_set(ERROR_CEILING_OR_FLOORING_UNDEFINED));
	game->environment.ceiling_color = ft_rgb_to_unsigned(ceiling, ",");
	game->environment.floor_color = ft_rgb_to_unsigned(flooring, ",");
}
