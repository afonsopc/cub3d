/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 19:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/11 19:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "brain.h"

void	brain_face(t_entity *e, t_coords t)
{
	if (fabs(t.x - e->coords.x) < 1e-6 && fabs(t.y - e->coords.y) < 1e-6)
		return ;
	e->coords.yaw = ft_normalize_angle(ft_degrees(atan2(t.y - e->coords.y,
					t.x - e->coords.x)));
}

static t_item	*weapon_of(t_character *c)
{
	int	i;

	i = -1;
	while (++i < INVENTORY_SIZE)
		if (c->inventory[i] && c->inventory[i]->weapon)
			return (c->inventory[i]);
	return (NULL);
}

bool	brain_is_melee(t_character *c)
{
	t_item	*w;

	w = weapon_of(c);
	return (w && ((t_weapon *)w)->range > 0 && ((t_weapon *)w)->range < 4);
}

/* keep a little space between guards */
static void	separate(t_game *game, t_character *c)
{
	t_entity	*e;
	t_entity	*o;
	double		cross;
	int			i;

	e = (t_entity *)c;
	i = -1;
	while (game->billboards && game->billboards[++i])
	{
		o = game->billboards[i];
		if (o == e || !brain_is_enemy(o) || ((t_character *)o)->dead
			|| ft_distance(e->coords, o->coords) > AI_SEPARATION)
			continue ;
		cross = ft_cos_degrees(e->coords.yaw) * (o->coords.y - e->coords.y)
			- ft_sin_degrees(e->coords.yaw) * (o->coords.x - e->coords.x);
		e->controller.walking_left = cross > 0;
		e->controller.walking_right = cross <= 0;
		return ;
	}
}

static void	unstick(t_character *c, t_brain *b)
{
	t_entity	*e;

	e = (t_entity *)c;
	if (brain_since(b->stuck_at) < AI_STUCK_CHECK_MS)
		return ;
	if (b->wants_move && ft_distance(e->coords, b->stuck_pos) < AI_STUCK_MIN_MOVE)
	{
		AI_LOG("AI %d stuck at %.2f %.2f, re-routing\n", b->id,
			e->coords.x, e->coords.y);
		b->has_path = false;
		b->strafe_until = ft_get_time() + 400;
		b->strafe_dir = (rand() % 2) * 2 - 1;
	}
	b->stuck_pos = e->coords;
	b->stuck_at = ft_get_time();
	b->wants_move = false;
}

/* returns true while waiting on a door that is opening */
static bool	door_ahead(t_game *game, t_character *c, t_brain *b)
{
	t_entity	*w;
	t_door		*door;

	w = game->walls[b->next_y][b->next_x];
	if (!w || w->type != ENTITY_DOOR)
		return (false);
	door = (t_door *)w;
	if (brain_door_open(door) >= AI_DOOR_OPEN_ENOUGH)
		return (false);
	brain_face((t_entity *)c, (t_coords){b->next_x + 0.5, b->next_y + 0.5, 0});
	if (!door->opened && door->wall.entity.action)
	{
		AI_LOG("AI %d opens door at %d %d\n", b->id, b->next_x, b->next_y);
		door->wall.entity.action(w, c);
	}
	return (true);
}

void	brain_walk_to(t_game *game, t_character *c, t_coords goal, bool sprint)
{
	t_entity	*e;
	t_brain		*b;
	t_coords	wp;

	e = (t_entity *)c;
	b = &e->controller.brain;
	unstick(c, b);
	wp = (t_coords){b->next_x + 0.5, b->next_y + 0.5, 0};
	if (!b->has_path || ft_distance(e->coords, wp) < AI_WAYPOINT_REACHED
		|| (brain_since(b->path_at) > AI_PATH_REFRESH_MS
			&& (int)e->coords.x == b->next_x && (int)e->coords.y == b->next_y))
	{
		if (!brain_next_step(game, c, goal))
			b->has_path = false;
		wp = (t_coords){b->next_x + 0.5, b->next_y + 0.5, 0};
	}
	if (!b->has_path)
		wp = goal;
	else if (door_ahead(game, c, b))
		return ;
	else if (b->blocked && ft_distance(e->coords, wp) > 0.6)
	{
		brain_face(e, wp);
		separate(game, c);
		return ;
	}
	brain_face(e, wp);
	e->controller.walking_forward = true;
	e->controller.sprinting = sprint;
	b->wants_move = true;
	separate(game, c);
	if (ft_get_time() < b->strafe_until)
	{
		e->controller.walking_left = b->strafe_dir < 0;
		e->controller.walking_right = b->strafe_dir > 0;
	}
}

static bool	halt_first(t_character *c)
{
	t_item	*h;

	if (c->inventory_index != 0 || !c->inventory[0] || c->inventory[0]->weapon)
		return (false);
	h = c->inventory[0];
	if (h->last_use && brain_since(h->last_use) < (long long)h->use_delay)
		return (true);
	h->user = NULL;
	if (c->inventory[1])
		c->inventory_index = 1;
	return (false);
}

static void	strafe(t_game *game, t_character *c, t_brain *b)
{
	t_entity	*e;
	double		side;
	int			sx;
	int			sy;

	e = (t_entity *)c;
	if (ft_get_time() >= b->next_strafe)
	{
		b->strafe_dir = (rand() % 2) * 2 - 1;
		b->strafe_until = ft_get_time() + AI_STRAFE_LENGTH_MS;
		b->next_strafe = ft_get_time() + AI_STRAFE_EVERY_MIN_MS
			+ rand() % (AI_STRAFE_EVERY_MAX_MS - AI_STRAFE_EVERY_MIN_MS);
	}
	if (ft_get_time() >= b->strafe_until)
		return ;
	side = e->coords.yaw + 90.0 * b->strafe_dir;
	sx = (int)(e->coords.x + ft_cos_degrees(side) * 0.7);
	sy = (int)(e->coords.y + ft_sin_degrees(side) * 0.7);
	if (!brain_cell_walkable(game, sx, sy, false)
		|| brain_cell_occupied(game, sx, sy, e))
		return ;
	e->controller.walking_left = b->strafe_dir < 0;
	e->controller.walking_right = b->strafe_dir > 0;
}

static double	accuracy(t_character *c, t_character *t, double d)
{
	t_controller	*tc;
	double			acc;

	if (brain_is_melee(c))
		return (AI_MELEE_ACCURACY);
	tc = &t->billboard.entity.controller;
	acc = AI_ACCURACY_BASE - AI_ACCURACY_PER_CELL * d;
	if (tc->walking_forward || tc->walking_backward || tc->walking_left
		|| tc->walking_right)
		acc -= AI_ACCURACY_MOVING_PENALTY;
	return (fmax(AI_ACCURACY_MIN, fmin(1.0, acc)));
}

void	brain_attack(t_game *game, t_character *c, t_character *t, bool sees)
{
	t_brain	*b;
	t_item	*w;
	double	d;
	bool	hit;

	b = &c->billboard.entity.controller.brain;
	brain_face((t_entity *)c, t->billboard.entity.coords);
	d = ft_distance(c->billboard.entity.coords, t->billboard.entity.coords);
	if (brain_is_melee(c) && d > 1.0)
		brain_walk_to(game, c, t->billboard.entity.coords, true);
	else if (!brain_is_melee(c))
		strafe(game, c, b);
	if (halt_first(c) || ft_get_time() < b->react_at || !sees)
		return ;
	w = c->inventory[c->inventory_index];
	if (!w || !w->weapon || brain_since(w->last_use) < (long long)w->use_delay)
		return ;
	hit = (double)rand() / RAND_MAX < accuracy(c, t, d);
	c->target_entity = NULL;
	if (hit)
		c->target_entity = (t_entity *)t;
	w->user = c;
	AI_LOG("AI %d fires at %.1f cells: %s\n", b->id, d, hit ? "hit" : "miss");
}
