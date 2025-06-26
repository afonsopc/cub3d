/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:05:46 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/26 12:45:31 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

static t_wall	*hit_wall(t_game *game, t_coords coords)
{
	t_wall	*wall;
	t_list	*curr;

	curr = game->entities;
	while (curr)
	{
		wall = curr->data;
		curr = curr->next;
		if (!wall->base.block || (int)wall->base.coords.x != (int)coords.x
			|| (int)wall->base.coords.y != (int)coords.y)
			continue ;
		return (wall);
	}
	return (NULL);
}

static void	init_dda_ray_data(t_dda_ray *data, t_coords coords)
{
	double	angle_rad;

	angle_rad = ft_normalize_angle(coords.yaw) * (PI / 180.0);
	data->ray_dir.x = cos(angle_rad);
	data->ray_dir.y = sin(angle_rad);
	data->map_pos.x = (int)coords.x;
	data->map_pos.y = (int)coords.y;
	if (data->ray_dir.x == 0)
		data->delta_dist.x = INFINITY;
	else
		data->delta_dist.x = fabs(1 / data->ray_dir.x);
	if (data->ray_dir.y == 0)
		data->delta_dist.y = INFINITY;
	else
		data->delta_dist.y = fabs(1 / data->ray_dir.y);
}

static void	set_step_and_side_dist(t_dda_ray *data, t_coords coords)
{
	if (data->ray_dir.x < 0)
	{
		data->step.x = -1;
		data->side_dist.x = (coords.x - data->map_pos.x) * data->delta_dist.x;
	}
	else
	{
		data->step.x = 1;
		data->side_dist.x = (data->map_pos.x + 1.0 - coords.x)
			* data->delta_dist.x;
	}
	if (data->ray_dir.y < 0)
	{
		data->step.y = -1;
		data->side_dist.y = (coords.y - data->map_pos.y) * data->delta_dist.y;
	}
	else
	{
		data->step.y = 1;
		data->side_dist.y = (data->map_pos.y + 1.0 - coords.y)
			* data->delta_dist.y;
	}
}

static double	perform_dda(t_dda_ray *data, t_game *game)
{
	t_coords	check_pos;

	while (1)
	{
		if (data->side_dist.x < data->side_dist.y)
		{
			data->side_dist.x += data->delta_dist.x;
			data->map_pos.x += data->step.x;
			data->side = 0;
		}
		else
		{
			data->side_dist.y += data->delta_dist.y;
			data->map_pos.y += data->step.y;
			data->side = 1;
		}
		check_pos = (t_coords){data->map_pos.x, data->map_pos.y, 0, 0};
		if (hit_wall(game, check_pos))
			break ;
		if (data->map_pos.x < 0 || data->map_pos.x >= game->map->size.width
			|| data->map_pos.y < 0 || data->map_pos.y >= game->map->size.height)
			return (PLAYER_RAYS_NO_HIT_LENGTH);
	}
	return (0);
}

t_raycast	send_ray(t_game *game, t_coords coords)
{
	t_dda_ray	data;
	double		result;
	t_wall		*wall_hit;

	init_dda_ray_data(&data, coords);
	set_step_and_side_dist(&data, coords);
	result = perform_dda(&data, game);
	if (result > 0)
		return ((t_raycast){result, NULL, 0, 0});
	data.length = calculate_wall_dist(&data, coords);
	wall_hit = hit_wall(game, (t_coords){data.map_pos.x, data.map_pos.y, 0, 0});
	calculate_wall_hit(&data, coords, wall_hit);
	return ((t_raycast){data.length, (t_entity *)wall_hit, data.wall_x,
		data.hit_direction});
}
