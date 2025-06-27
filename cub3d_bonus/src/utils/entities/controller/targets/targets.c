/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   targets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:42:39 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:23:26 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "targets.h"

double	signed_angle_to(t_coords from, t_coords to)
{
	double	angle_to;
	double	diff;

	angle_to = ft_degrees(atan2(to.y - from.y, to.x - from.x));
	diff = angle_to - from.yaw;
	while (diff > 180.0)
		diff -= 360.0;
	while (diff < -180.0)
		diff += 360.0;
	return (diff);
}

void	targets_frame(t_game *game, t_character *character, double fov)
{
	t_wall_target	wall_tar_data;
	t_entity		*bill_tar_entity;

	if (!character)
		return ;
	character->target_entity = NULL;
	wall_tar_data = wall_target(game, character, fov);
	if (wall_tar_data.entity)
		character->target_entity = wall_tar_data.entity;
	bill_tar_entity = billboard_target(game, (t_entity *)character, fov);
	if (bill_tar_entity)
		character->target_entity = bill_tar_entity;
}
