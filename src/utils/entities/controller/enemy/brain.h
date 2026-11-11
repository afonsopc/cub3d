/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 19:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/11 19:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BRAIN_H
# define BRAIN_H

# include "enemy.h"
# include "ai_tuning.h"
# include <stdio.h>
# include <string.h>

typedef enum e_brain_state
{
	BRAIN_IDLE,
	BRAIN_SEARCH,
	BRAIN_RETURN,
	BRAIN_CHASE,
	BRAIN_ATTACK,
	BRAIN_PAIN,
	BRAIN_DEAD
}	t_brain_state;

# ifdef AI_TRACE
#  define AI_LOG(...) printf(__VA_ARGS__)
# else
#  define AI_LOG(...) ((void)0)
# endif

/* brain0.c: states */
void		enemy_brain_frame(t_game *game, t_entity *entity, double dt);
void		brain_set_state(t_character *c, int state);
const char	*brain_state_name(int state);
long long	brain_since(t_time t);

/* brain1.c: grid, paths, sight */
bool		brain_cell_walkable(t_game *game, int x, int y, bool doors_too);
bool		brain_next_step(t_game *game, t_character *c, t_coords goal);
bool		brain_can_see(t_game *game, t_character *c, t_character *target);
t_player	*brain_nearest_player(t_game *game, t_character *c);

/* brain2.c: hearing, doors */
void		brain_hear(t_character *c, t_coords origin);
bool		brain_cell_occupied(t_game *game, int x, int y, t_entity *except);
double		brain_door_open(t_door *door);
bool		brain_is_enemy(t_entity *entity);

/* brain3.c: moving and fighting */
void		brain_walk_to(t_game *game, t_character *c, t_coords goal,
				bool sprint);
void		brain_face(t_entity *e, t_coords target);
void		brain_attack(t_game *game, t_character *c, t_character *target,
				bool sees);
bool		brain_is_melee(t_character *c);

#endif
