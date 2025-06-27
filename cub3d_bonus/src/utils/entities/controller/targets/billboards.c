/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   billboards.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:43:23 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:22:18 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "targets.h"

static bool	billboard_inside_threshold(double dist, t_coords coords,
	t_entity *entity, double fov)
{
	bool	inside_aim_threshold;
	bool	inside_distance_threshold;

	inside_aim_threshold = fabs(signed_angle_to(coords, entity->coords)) > fov
		/ (dist + 1.0);
	inside_distance_threshold = dist > TARGETS_MAX_DIST;
	return (inside_aim_threshold || inside_distance_threshold);
}

static inline bool	is_door_and_opened(t_entity *entity)
{
	return (entity->type == ENTITY_DOOR && ((t_door *)entity)->opened);
}

static bool	is_valid_target(t_entity *entity, t_entity *billboard, double dist,
	double fov)
{
	if (billboard == entity || !billboard->targetable || !billboard->active)
		return (false);
	return (!billboard_inside_threshold(dist, entity->coords, billboard, fov));
}

static bool	has_line_of_sight(t_game *game, t_entity *entity,
							t_entity *target, double target_dist)
{
	t_raycast	los_ray;
	double		angle_to_target;

	angle_to_target = ft_degrees(atan2(target->coords.y - entity->coords.y,
				target->coords.x - entity->coords.x));
	los_ray = ft_dda_raycast((t_dda_raycast_config){
			(void ***)game->walls, game->map->size,
			(t_coords){
			entity->coords.x, entity->coords.y,
			ft_normalize_angle(angle_to_target)
		}, entity});
	if (!los_ray.hit)
		return (true);
	if (is_door_and_opened(los_ray.hit))
	{
		if (target_dist - los_ray.distance <= 0.1)
			return (true);
		return (has_line_of_sight(game, los_ray.hit, target, target_dist
				- los_ray.distance));
	}
	return (los_ray.distance >= target_dist);
}

t_entity	*billboard_target(t_game *game, t_entity *entity, double fov)
{
	t_entity	*closest;
	t_entity	*bill;
	double		dist;
	int			i;

	closest = NULL;
	if (!game->billboards)
		return (NULL);
	i = -1;
	while (game->billboards[++i])
	{
		bill = game->billboards[i];
		dist = ft_distance(entity->coords, bill->coords);
		if (!is_valid_target(entity, bill, dist, fov))
			continue ;
		if (!has_line_of_sight(game, entity, bill, dist))
			continue ;
		closest = bill;
	}
	return (closest);
}
