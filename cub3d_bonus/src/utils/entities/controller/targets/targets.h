/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   targets.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:42:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/16 17:01:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TARGETS_H
# define TARGETS_H

# include "../controller.h"

# define TARGETS_FOV 70.0
# define TARGETS_RAYS 3
# define TARGETS_MAX_DIST 100.0

typedef struct s_wall_target
{
	t_entity	*entity;
	t_direction	direction;
}	t_wall_target;

double			signed_angle_to(t_coords from, t_coords to);

t_entity		*billboard_target(t_game *game, t_entity *entity, double fov);
t_wall_target	wall_target(t_game *game, t_character *character, double fov);

#endif