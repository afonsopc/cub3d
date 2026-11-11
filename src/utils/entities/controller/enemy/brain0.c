/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 19:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/11 19:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "brain.h"

/*
** Enemy brain, in the spirit of the early-90s shooters:
** idle -> (sees you in its cone, or hears a gunshot) -> chase / search
** chase -> attack (in range with line of sight) -> chase when you hide
** search: walk to the noise / last seen spot, look around, give up, return
** pain: a short stagger when hit, then it knows where the shot came from.
*/

long long	brain_since(t_time t)
{
	return ((long long)ft_get_time() - (long long)t);
}

const char	*brain_state_name(int state)
{
	static const char	*names[] = {"idle", "search", "return", "chase",
		"attack", "pain", "dead"};

	if (state < 0 || state > BRAIN_DEAD)
		return ("?");
	return (names[state]);
}

void	brain_set_state(t_character *c, int state)
{
	t_brain	*b;

	b = &c->billboard.entity.controller.brain;
	if (b->state == state)
		return ;
	AI_LOG("AI %d %c %s->%s at %.2f %.2f\n", b->id,
		c->billboard.entity.identifier, brain_state_name(b->state),
		brain_state_name(state), c->billboard.entity.coords.x,
		c->billboard.entity.coords.y);
	b->state = state;
	b->state_at = ft_get_time();
	b->has_path = false;
	b->arrived_at = 0;
}

static void	first_tick(t_character *c, t_brain *b)
{
	static int	next_id;

	b->home = c->billboard.entity.coords;
	b->home_set = true;
	b->last_health = c->billboard.entity.health;
	b->id = ++next_id;
	b->stuck_pos = c->billboard.entity.coords;
	b->stuck_at = ft_get_time();
	b->next_strafe = ft_get_time() + AI_STRAFE_EVERY_MIN_MS;
	if (brain_is_melee(c))
		c->billboard.entity.controller.sprint_velocity *= 1.35;
	AI_LOG("AI %d %c spawn at %.2f %.2f\n", b->id,
		c->billboard.entity.identifier, b->home.x, b->home.y);
}

static void	sees_target(t_character *c, t_brain *b, t_character *t)
{
	double	d;

	b->goal = t->billboard.entity.coords;
	b->knows_target = true;
	b->seen_at = ft_get_time();
	if (b->state == BRAIN_CHASE || b->state == BRAIN_ATTACK
		|| b->state == BRAIN_PAIN)
		return ;
	d = ft_distance(c->billboard.entity.coords, t->billboard.entity.coords);
	b->react_at = ft_get_time() + AI_REACTION_MS
		+ (t_time)(d * AI_REACTION_PER_CELL_MS)
		+ (t_time)(rand() % AI_REACTION_RANDOM_MS);
	AI_LOG("AI %d sees player at %.1f cells\n", b->id, d);
	brain_set_state(c, BRAIN_CHASE);
	if (!b->halted && c->inventory[0] && !c->inventory[0]->weapon)
	{
		c->inventory_index = 0;
		c->inventory[0]->user = c;
	}
	b->halted = true;
}

static void	pain_check(t_character *c, t_brain *b)
{
	int	health;

	health = c->billboard.entity.health;
	if (health >= b->last_health)
	{
		b->last_health = health;
		return ;
	}
	b->last_health = health;
	if (c->last_hit_by_character)
	{
		b->goal = c->last_hit_by_character->billboard.entity.coords;
		b->knows_target = true;
		b->seen_at = ft_get_time();
		brain_face((t_entity *)c, b->goal);
	}
	b->halted = true;
	if (b->react_at < ft_get_time() + AI_PAIN_MS)
		b->react_at = ft_get_time() + AI_PAIN_MS + 120;
	brain_set_state(c, BRAIN_PAIN);
}

static void	do_search(t_game *game, t_character *c, t_brain *b)
{
	t_entity	*e;

	e = (t_entity *)c;
	if (!b->arrived_at && ft_distance(e->coords, b->goal) > 0.6)
	{
		brain_walk_to(game, c, b->goal, false);
		if (brain_since(b->state_at) > AI_SEARCH_GIVE_UP_MS)
			brain_set_state(c, BRAIN_RETURN);
		return ;
	}
	if (!b->arrived_at)
	{
		b->arrived_at = ft_get_time();
		AI_LOG("AI %d reached search spot, looking around\n", b->id);
	}
	e->controller.looking_right = true;
	if (brain_since(b->arrived_at) > AI_SEARCH_LOOK_MS)
		brain_set_state(c, BRAIN_RETURN);
}

static void	do_return(t_game *game, t_character *c, t_brain *b)
{
	t_entity	*e;

	e = (t_entity *)c;
	if (ft_distance(e->coords, b->home) > 0.35)
	{
		brain_walk_to(game, c, b->home, false);
		if (brain_since(b->state_at) > AI_SEARCH_GIVE_UP_MS * 2)
			brain_set_state(c, BRAIN_IDLE);
		return ;
	}
	e->coords.yaw = b->home.yaw;
	b->knows_target = false;
	b->halted = false;
	brain_set_state(c, BRAIN_IDLE);
}

static double	attack_range(t_character *c)
{
	int	i;

	if (!brain_is_melee(c))
		return (AI_ATTACK_RANGE);
	i = -1;
	while (++i < INVENTORY_SIZE)
		if (c->inventory[i] && c->inventory[i]->weapon)
			return (((t_weapon *)c->inventory[i])->range * 0.85);
	return (1.5);
}

static void	do_chase(t_game *game, t_character *c, t_brain *b, t_character *p)
{
	double	d;
	bool	sees;

	sees = p && b->sees;
	d = ft_distance(c->billboard.entity.coords, b->goal);
	if (sees && d <= attack_range(c))
		return (brain_set_state(c, BRAIN_ATTACK));
	if (!sees && brain_since(b->seen_at) > AI_LOSE_SIGHT_MS)
		return (brain_set_state(c, BRAIN_SEARCH));
	brain_walk_to(game, c, b->goal, brain_is_melee(c) || d > 6);
}

static void	do_attack(t_game *game, t_character *c, t_brain *b, t_character *p)
{
	bool	sees;

	sees = p && b->sees;
	if (!p || (!sees && brain_since(b->seen_at) > 400))
		return (brain_set_state(c, BRAIN_CHASE));
	if (ft_distance(c->billboard.entity.coords, p->billboard.entity.coords)
		> attack_range(c) + 1.5)
		return (brain_set_state(c, BRAIN_CHASE));
	brain_attack(game, c, p, sees);
}

static void	think(t_game *game, t_character *c, t_brain *b, t_character *p)
{
	if (b->state == BRAIN_PAIN && brain_since(b->state_at) > AI_PAIN_MS)
	{
		if (b->knows_target)
			brain_set_state(c, BRAIN_CHASE);
		else
			brain_set_state(c, BRAIN_SEARCH);
	}
	if ((b->state == BRAIN_CHASE || b->state == BRAIN_ATTACK) && !p)
		brain_set_state(c, BRAIN_RETURN);
	if (b->state == BRAIN_SEARCH)
		do_search(game, c, b);
	else if (b->state == BRAIN_RETURN)
		do_return(game, c, b);
	else if (b->state == BRAIN_CHASE)
		do_chase(game, c, b, p);
	else if (b->state == BRAIN_ATTACK)
		do_attack(game, c, b, p);
}

void	enemy_brain_frame(t_game *game, t_entity *entity, double dt)
{
	t_character	*c;
	t_brain		*b;
	t_character	*p;

	if (!entity->character)
		return ;
	c = (t_character *)entity;
	b = &entity->controller.brain;
	remove_attributes(c, &entity->controller);
	if (c->dead)
		return (brain_set_state(c, BRAIN_DEAD));
	if (!b->home_set)
		first_tick(c, b);
	pain_check(c, b);
	p = (t_character *)brain_nearest_player(game, c);
	b->sees = p && brain_can_see(game, c, p);
	if (b->sees)
		sees_target(c, b, p);
	think(game, c, b, p);
	moviment_frame(game, entity, dt);
}
