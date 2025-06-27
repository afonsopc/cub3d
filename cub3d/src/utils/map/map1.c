/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 08:42:58 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 13:59:43 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

static void	read_map_raw_lines_e(t_map *map)
{
	int		fd;
	char	*line;
	char	**tmp_line;
	char	**tmp_map;

	fd = open(map->path, O_RDONLY);
	if (fd < 0)
		return (fte_set(ERROR_MAP_OPEN));
	while (1)
	{
		line = ft_get_next_line(fd);
		if (!line)
			break ;
		if (ft_str_endswith(line, "\n"))
			line[ft_strlen(line) - 1] = '\0';
		tmp_map = map->raw;
		tmp_line = ft_strvndup(&line, 1);
		map->raw = ft_strvjoin(map->raw, tmp_line);
		(ft_strvfree(tmp_map), ft_strvfree(tmp_line), free(line));
	}
	ft_close(fd);
	if (!map->raw)
		return (fte_set(ERROR_MAP_READ));
	fte_set(ERROR_NO_ERROR);
}

t_map	*parse_map_e(char *path)
{
	t_map	*map;

	fte_set(ERROR_NO_ERROR);
	if (!path)
		return (fte_set(ERROR_INVALID_MAP), NULL);
	map = ft_calloc(1, sizeof(t_map));
	if (!map)
		return (fte_set(ERROR_MAP_ALLOC), NULL);
	map->path = ft_strdup(path);
	if (!map->path || !ft_str_endswith(map->path, ".cub"))
		return (fte_set(ERROR_INVALID_FILETYPE), destroy_map(map), NULL);
	read_map_raw_lines_e(map);
	if (fte_flagged())
		return (destroy_map(map), NULL);
	process_raw_map_e(map);
	if (fte_flagged())
		return (destroy_map(map), NULL);
	parse_identifiers_e(map);
	if (fte_flagged())
		return (destroy_map(map), NULL);
	set_map_size(map);
	return (map);
}

void	destroy_map(t_map *map)
{
	if (!map)
		return ;
	ft_strvfree(map->raw);
	ft_hashmap_destroy(map->types);
	free(map->path);
	free(map);
}
