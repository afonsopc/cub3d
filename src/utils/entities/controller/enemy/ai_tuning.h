/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_tuning.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 19:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/11 19:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Every tuning knob of the enemy AI, in one place. Distances are in map
** cells, times in milliseconds, angles in degrees.
*/

#ifndef AI_TUNING_H
# define AI_TUNING_H

/* hearing: gunshots flood through floor cells and open doors */
# define AI_HEARING_RADIUS 12

/* sight: cone in front of the enemy, plus line of sight */
# define AI_VIEW_CONE 120.0
# define AI_VIEW_DISTANCE 24.0
/* seconds of "memory" after losing sight before searching */
# define AI_LOSE_SIGHT_MS 2500

/* reaction before the first shot: base + per cell of distance + random */
# define AI_REACTION_MS 350
# define AI_REACTION_PER_CELL_MS 35
# define AI_REACTION_RANDOM_MS 250

/* ranged attack */
# define AI_ATTACK_RANGE 11.0
# define AI_ACCURACY_BASE 0.80
# define AI_ACCURACY_PER_CELL 0.045
# define AI_ACCURACY_MOVING_PENALTY 0.25
# define AI_ACCURACY_MIN 0.10
# define AI_MELEE_ACCURACY 0.85

/* strafing while shooting */
# define AI_STRAFE_EVERY_MIN_MS 1600
# define AI_STRAFE_EVERY_MAX_MS 3600
# define AI_STRAFE_LENGTH_MS 450

/* pain, search, give up */
# define AI_PAIN_MS 280
# define AI_SEARCH_LOOK_MS 3500
# define AI_SEARCH_GIVE_UP_MS 14000
# define AI_LOOK_AROUND_SPEED 90.0

/* paths and getting unstuck */
# define AI_PATH_REFRESH_MS 350
# define AI_STUCK_CHECK_MS 900
# define AI_STUCK_MIN_MOVE 0.12
# define AI_SEPARATION 0.75
# define AI_WAYPOINT_REACHED 0.18

/* chase speed (the dog sprints) */
# define AI_DOG_SPRINTS 1

/* the door a guard waits on is open enough to walk through */
# define AI_DOOR_OPEN_ENOUGH 0.95

#endif
