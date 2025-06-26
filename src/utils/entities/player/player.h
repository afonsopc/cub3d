/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:09:30 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/26 12:47:48 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include "../entities.h"
# include <cub3d.h>

typedef struct s_dda_ray
{
	double		length;
	double		angle;
	t_coords	ray_dir;
	t_coords	delta_dist;
	t_coords	side_dist;
	t_coords	map_pos;
	t_coords	step;
	int			side;
	double		wall_x;
	t_entity	*hit_entity;
	t_direction	hit_direction;
}				t_dda_ray;

typedef struct s_raycast
{
	double		length;
	t_entity	*hit_entity;
	double		x_of_hit_in_entity;
	t_direction	hit_direction;
}				t_raycast;

t_raycast	send_ray(t_game *game, t_coords coords);
double		calculate_wall_dist(t_dda_ray *data, t_coords coords);
void		calculate_wall_hit(t_dda_ray *data, t_coords coords,
				t_wall *wall_hit);

#endif