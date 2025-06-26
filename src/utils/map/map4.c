/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 13:18:07 by pvcordeiro        #+#    #+#             */
/*   Updated: 2025/06/26 13:56:56 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

static bool	check_map_boundary(char **map_copy, int x, int y, t_map *map)
{
	int	new_x;
	int	new_y;

	new_x = x + 1;
	new_y = y;
	if (new_x < 0 || new_y < 0 || new_y >= map->size.height
		|| !map_copy[new_y][new_x] || map_copy[new_y][new_x] == ' ')
		return (false);
	new_x = x - 1;
	if (new_x < 0 || !map_copy[new_y][new_x] || map_copy[new_y][new_x] == ' ')
		return (false);
	new_x = x;
	new_y = y + 1;
	if (new_y < 0 || new_y >= map->size.height
		|| !map_copy[new_y][new_x] || map_copy[new_y][new_x] == ' ')
		return (false);
	new_y = y - 1;
	if (new_y < 0 || !map_copy[new_y][new_x] || map_copy[new_y][new_x] == ' ')
		return (false);
	return (true);
}

static bool	validate_textures(t_game *game)
{
	char	*required_textures[4];
	int		i;

	required_textures[0] = "NO";
	required_textures[1] = "SO";
	required_textures[2] = "WE";
	required_textures[3] = "EA";
	i = -1;
	while (++i < 4)
		if (!ft_hashmap_get(game->sprites, required_textures[i]))
			return (false);
	return (true);
}

static bool	validate_player_position(t_map *map, int *player_x, int *player_y)
{
	int		player_count;
	int		x;
	int		y;

	player_count = 0;
	*player_x = 0;
	*player_y = 0;
	y = -1;
	while (++y < map->size.height)
	{
		x = -1;
		while (map->map[y][++x])
		{
			if (!ft_strchr(DEFAULT_MAP_TYPES, map->map[y][x]))
				return (false);
			if (ft_strchr(DEFAULT_PLAYER_TYPES, map->map[y][x]))
			{
				player_count += 1;
				*player_x = x;
				*player_y = y;
			}
		}
	}
	return (player_count == 1);
}

static bool	validate_map_boundaries(char **map_copy, t_map *map)
{
	int	x;
	int	y;

	y = -1;
	while (++y < map->size.height)
	{
		x = -1;
		while (map_copy[y][++x])
		{
			if (map_copy[y][x] == 'F' && !check_map_boundary(map_copy, x, y,
					map))
				return (false);
		}
	}
	return (true);
}

void	ft_backtrack_e(t_game *game)
{
	int		player_x;
	int		player_y;
	t_map	*map;
	char	**map_copy;

	map = game->map;
	fte_set(ERROR_NO_ERROR);
	if (!validate_textures(game))
		return (fte_set(ERROR_INVALID_MAP));
	if (!validate_player_position(map, &player_x, &player_y))
		return (fte_set(ERROR_INVALID_MAP));
	map_copy = ft_strvdup(map->map);
	if (!map_copy)
		return (fte_set(ERROR_MAP_ALLOC));
	map_copy[player_y][player_x] = '0';
	flood_fill_map(map_copy, player_x, player_y, map->size);
	if (!validate_map_boundaries(map_copy, map))
	{
		ft_strvfree(map_copy);
		return (fte_set(ERROR_INVALID_MAP));
	}
	ft_strvfree(map_copy);
}
