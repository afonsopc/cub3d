/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:39:13 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:51:08 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_H
# define ENEMY_H

# include "../controller.h"

# define STRAFE_INTERVAL 2500
# define FIRE_CHANCE 0.05
# define LOOK_AROUND_CHANCE 0.01
# define SHOT_HEARING_RANGE 10
# define SPRINT_HEARING_RANGE 5
# define FIRST_SHOT_DELAY 500
# define LAST_HIT_DELAY 1000

void	look_at_target(t_entity *entity, t_entity *target_entity);
void	update_movement(t_entity *entity, t_entity *target_entity);
void	remove_attributes(t_character *character, t_controller *controller);

bool	do_has_seen_in_short_notice(t_controller *controller,
			t_character *character);
void	look_around(t_character *character, t_controller *controller);
void	do_has_heard(t_game *game, t_character *character, t_player *player);
void	hearment(t_game *game, t_character *character);
void	shoot(t_character *character);

#endif
