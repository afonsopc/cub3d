/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map5.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:49:52 by pvcordeiro        #+#    #+#             */
/*   Updated: 2025/06/26 12:52:46 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

void	flood_fill_map(char **map, int x, int y, t_size m_size)
{
	if (x < 0 || y < 0 || x > m_size.width || y > m_size.height || !map[y]
		|| !map[y][x] || map[y][x] == 'F' || map[y][x] == '1'
		|| map[y][x] == ' ')
		return ;
	map[y][x] = 'F';
	flood_fill_map(map, x + 1, y, m_size);
	flood_fill_map(map, x - 1, y, m_size);
	flood_fill_map(map, x, y + 1, m_size);
	flood_fill_map(map, x, y - 1, m_size);
}
