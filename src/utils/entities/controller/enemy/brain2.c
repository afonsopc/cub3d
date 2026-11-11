/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 19:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/11 19:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "brain.h"

bool	brain_is_enemy(t_entity *entity)
{
	return (entity && entity->character
		&& entity->controller.frame == enemy_brain_frame);
}

double	brain_door_open(t_door *door)
{
	if (door->opened && !door->wall.entity.hard)
		return (1.0);
	if (!door->opening_sprite || door->animation_frames <= 1)
		return (0.0);
	return (fmin(1.0, fmax(0.0, (double)door->opening_sprite->index
				/ (door->animation_frames - 1))));
}

bool	brain_cell_occupied(t_game *game, int x, int y, t_entity *except)
{
	t_entity	*e;
	int			i;

	i = -1;
	while (game->billboards && game->billboards[++i])
	{
		e = game->billboards[i];
		if (e == except || !e->character || e->type == ENTITY_PLAYER
			|| ((t_character *)e)->dead)
			continue ;
		if ((int)e->coords.x == x && (int)e->coords.y == y)
			return (true);
	}
	return (false);
}

void	brain_hear(t_character *c, t_coords origin)
{
	t_brain	*b;

	b = &c->billboard.entity.controller.brain;
	if (c->dead || !b->home_set)
		return ;
	if (b->state == BRAIN_CHASE || b->state == BRAIN_ATTACK
		|| b->state == BRAIN_PAIN)
	{
		if (!b->sees)
			b->goal = origin;
		return ;
	}
	AI_LOG("AI %d %c heard a shot from %.1f %.1f\n", b->id,
		c->billboard.entity.identifier, origin.x, origin.y);
	b->goal = origin;
	brain_set_state(c, BRAIN_SEARCH);
}

static bool	sound_passes(t_game *game, int x, int y)
{
	t_entity	*e;

	e = game->walls[y][x];
	if (!e)
		return (true);
	return (e->type == ENTITY_DOOR && ((t_door *)e)->opened);
}

static void	alert_reached(t_game *game, int *depth, int w, t_coords origin)
{
	t_entity	*e;
	int			i;
	int			n;

	n = 0;
	i = -1;
	while (game->billboards && game->billboards[++i])
	{
		e = game->billboards[i];
		if (!brain_is_enemy(e) || ((t_character *)e)->dead
			|| e->coords.x < 0 || e->coords.y < 0
			|| (int)e->coords.x >= w
			|| (int)e->coords.y >= game->map->size.height
			|| depth[(int)e->coords.y * w + (int)e->coords.x] < 0)
			continue ;
		brain_hear((t_character *)e, origin);
		n++;
	}
	AI_LOG("AI noise at %.1f %.1f reached %d enemies\n", origin.x, origin.y, n);
	(void)n;
}

/*
** A gunshot: flood fill through floor and open doors, up to the radius.
** Guards that hear it go to where the fight is: the shooter when it is the
** player, or the player position the shooting guard knows about.
*/
void	ai_noise(t_game *game, t_character *shooter)
{
	t_coords	origin;
	t_coords	from;

	static int	*depth;
	static int	*queue;
	static int	cap;
	int			w;
	int			h;
	int			head;
	int			tail;
	int			k;

	if (!game || !game->map || !game->walls || !shooter)
		return ;
	origin = shooter->billboard.entity.coords;
	from = origin;
	if (brain_is_enemy((t_entity *)shooter))
	{
		if (!shooter->billboard.entity.controller.brain.knows_target)
			return ;
		origin = shooter->billboard.entity.controller.brain.goal;
	}
	w = game->map->size.width;
	h = game->map->size.height;
	if ((int)from.x < 0 || (int)from.y < 0 || (int)from.x >= w
		|| (int)from.y >= h)
		return ;
	if (cap < w * h)
	{
		free(depth);
		free(queue);
		depth = malloc(sizeof(int) * w * h);
		queue = malloc(sizeof(int) * w * h);
		cap = w * h;
		if (!depth || !queue)
			return ((void)(cap = 0));
	}
	memset(depth, -1, sizeof(int) * w * h);
	head = 0;
	tail = 0;
	k = (int)from.y * w + (int)from.x;
	depth[k] = 0;
	queue[tail++] = k;
	while (head < tail)
	{
		k = queue[head++];
		if (depth[k] >= AI_HEARING_RADIUS)
			continue ;
		for (int i = 0; i < 4; i++)
		{
			int	nx = k % w + (i == 0) - (i == 1);
			int	ny = k / w + (i == 2) - (i == 3);

			if (nx < 0 || ny < 0 || nx >= w || ny >= h
				|| depth[ny * w + nx] >= 0 || !sound_passes(game, nx, ny))
				continue ;
			depth[ny * w + nx] = depth[k] + 1;
			queue[tail++] = ny * w + nx;
		}
	}
	alert_reached(game, depth, w, origin);
}
