/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 08:42:58 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 21:03:47 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

static char	**get_last_empty_line(char **raw)
{
	int	i;

	i = ft_strvlen(raw);
	while (raw[--i])
		if (ft_str_all(raw[i], ft_isspace))
			return (&raw[i]);
	return (raw);
}

static void	add_variable_to_types_e(t_hashmap *types, char *line)
{
	size_t	i;
	char	*clean_line;
	char	*key;
	char	*value;

	clean_line = ft_strtrim(line);
	i = 0;
	while (ft_isspace(clean_line[i]))
		++i;
	key = &clean_line[i];
	while (!ft_isspace(clean_line[i]))
		++i;
	if (!clean_line[i])
		return (free(clean_line), fte_set("invalid type"));
	clean_line[i++] = '\0';
	if (ft_hashmap_get(types, key))
		return (free(clean_line), fte_set("duplicate type %s", line));
	while (ft_isspace(clean_line[i]))
		++i;
	value = ft_strdup(&clean_line[i]);
	ft_hashmap_set(types, key, value, free);
	free(clean_line);
	fte_set(NULL);
}

void	process_raw_map_e(t_map *map)
{
	size_t	i;

	map->types = ft_hashmap_new();
	if (!map->types)
		return (fte_set("map hashmap"));
	map->map = get_last_empty_line(map->raw);
	i = 0;
	while (map->raw[i] && &map->raw[i] != map->map)
	{
		if (!ft_str_all(map->raw[i], ft_isspace))
		{
			add_variable_to_types_e(map->types, map->raw[i]);
			if (fte_flagged())
				return ;
		}
		++i;
	}
}

void	set_map_size(t_map *map)
{
	int		i;
	size_t	biggest;
	size_t	tmp;

	i = -1;
	biggest = 0;
	while (map->map[++i])
	{
		tmp = ft_strlen(map->map[i]);
		if (tmp > biggest)
			biggest = tmp;
	}
	map->size.width = biggest;
	map->size.height = i;
}
