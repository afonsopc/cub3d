/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:44:52 by pvcordeiro        #+#    #+#             */
/*   Updated: 2025/06/26 12:45:35 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

double	calculate_wall_dist(t_dda_ray *data, t_coords coords)
{
	double	wall_dist;

	if (data->side == 0)
	{
		wall_dist = (data->map_pos.x - coords.x + (1 - data->step.x) / 2)
			/ data->ray_dir.x;
		if (data->step.x > 0)
			data->hit_direction = WEST;
		else
			data->hit_direction = EAST;
	}
	else
	{
		wall_dist = (data->map_pos.y - coords.y + (1 - data->step.y) / 2)
			/ data->ray_dir.y;
		if (data->step.y > 0)
			data->hit_direction = NORTH;
		else
			data->hit_direction = SOUTH;
	}
	return (wall_dist);
}

void	calculate_wall_hit(t_dda_ray *data, t_coords coords,
		t_wall *wall_hit)
{
	data->hit_entity = (t_entity *)wall_hit;
	if (data->side == 0)
		data->wall_x = coords.y + data->length * data->ray_dir.y;
	else
		data->wall_x = coords.x + data->length * data->ray_dir.x;
	data->wall_x -= floor(data->wall_x);
}
