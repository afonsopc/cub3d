/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:04:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/14 15:44:07 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include <cub3d.h>

void	process_raw_map_e(t_map *map);
void	set_map_size(t_map *map);
void	parse_identifiers_e(t_map *map);

#endif