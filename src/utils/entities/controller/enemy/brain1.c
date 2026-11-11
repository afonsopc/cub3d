/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brain1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 19:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/11 19:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "brain.h"

/* grid buffers shared by all enemies (logic runs on the main thread) */
typedef struct s_brain_grid
{
	t_game	*game;
	int		w;
	int		h;
	bool	*furniture;
	int		*dist;
	int		*queue;
}	t_brain_grid;

static t_brain_grid	*grid(t_game *game)
{
	static t_brain_grid	g;
	int					i;
	t_entity			*e;

	if (g.game == game && g.w == game->map->size.width
		&& g.h == game->map->size.height)
		return (&g);
	free(g.furniture);
	free(g.dist);
	free(g.queue);
	g.game = game;
	g.w = game->map->size.width;
	g.h = game->map->size.height;
	g.furniture = ft_calloc(g.w * g.h, sizeof(bool));
	g.dist = malloc(sizeof(int) * g.w * g.h);
	g.queue = malloc(sizeof(int) * g.w * g.h);
	i = -1;
	while (g.furniture && game->billboards && game->billboards[++i])
	{
		e = game->billboards[i];
		if (e->type == ENTITY_BILLBOARD && e->hard && (int)e->coords.x >= 0
			&& (int)e->coords.x < g.w && (int)e->coords.y >= 0
			&& (int)e->coords.y < g.h)
			g.furniture[(int)e->coords.y * g.w + (int)e->coords.x] = true;
	}
	return (&g);
}

bool	brain_cell_walkable(t_game *game, int x, int y, bool doors_too)
{
	t_entity		*e;
	t_brain_grid	*g;

	g = grid(game);
	if (x < 0 || y < 0 || x >= g->w || y >= g->h)
		return (false);
	e = game->walls[y][x];
	if (!e)
		return (!g->furniture || !g->furniture[y * g->w + x]);
	if (e->type != ENTITY_DOOR)
		return (false);
	if (((t_door *)e)->closeable && doors_too)
		return (true);
	return (((t_door *)e)->opened && !e->hard);
}

static int	bfs_from_goal(t_game *game, t_character *c, int gidx, bool crowd)
{
	t_brain_grid	*g;
	int				head;
	int				tail;
	int				k;
	int				n[4];

	g = grid(game);
	memset(g->dist, -1, sizeof(int) * g->w * g->h);
	head = 0;
	tail = 0;
	g->dist[gidx] = 0;
	g->queue[tail++] = gidx;
	while (head < tail)
	{
		k = g->queue[head++];
		n[0] = k - 1;
		n[1] = k + 1;
		n[2] = k - g->w;
		n[3] = k + g->w;
		for (int i = 0; i < 4; i++)
		{
			if (n[i] < 0 || n[i] >= g->w * g->h || g->dist[n[i]] != -1
				|| (i < 2 && n[i] / g->w != k / g->w)
				|| !brain_cell_walkable(game, n[i] % g->w, n[i] / g->w, true))
				continue ;
			g->dist[n[i]] = g->dist[k] + 1;
			if (n[i] == (int)c->billboard.entity.coords.y * g->w
				+ (int)c->billboard.entity.coords.x)
				return (g->dist[n[i]]);
			if (crowd && brain_cell_occupied(game, n[i] % g->w, n[i] / g->w,
					(t_entity *)c))
				continue ;
			g->queue[tail++] = n[i];
		}
	}
	return (-1);
}

/* picks the next grid cell towards goal (4-neighbour BFS, no corner cuts) */
bool	brain_next_step(t_game *game, t_character *c, t_coords goal)
{
	t_brain_grid	*g;
	t_brain			*b;
	int				s;
	int				best;
	int				n[4];

	g = grid(game);
	b = &c->billboard.entity.controller.brain;
	s = (int)c->billboard.entity.coords.y * g->w
		+ (int)c->billboard.entity.coords.x;
	if ((int)goal.x < 0 || (int)goal.y < 0 || (int)goal.x >= g->w
		|| (int)goal.y >= g->h || s == (int)goal.y * g->w + (int)goal.x)
		return (false);
	if (bfs_from_goal(game, c, (int)goal.y * g->w + (int)goal.x, true) < 0
		&& bfs_from_goal(game, c, (int)goal.y * g->w + (int)goal.x, false) < 0)
		return (false);
	n[0] = s - 1;
	n[1] = s + 1;
	n[2] = s - g->w;
	n[3] = s + g->w;
	best = -1;
	b->blocked = true;
	for (int i = 0, o = rand() % 4; i < 4; i++)
	{
		int	k = n[(i + o) % 4];

		if (k < 0 || k >= g->w * g->h || g->dist[k] < 0
			|| g->dist[k] != g->dist[s] - 1)
			continue ;
		if (best < 0 || b->blocked)
		{
			best = k;
			b->blocked = brain_cell_occupied(game, k % g->w, k / g->w,
					(t_entity *)c);
		}
	}
	if (best < 0)
		return (false);
	b->next_x = best % g->w;
	b->next_y = best / g->w;
	b->has_path = true;
	b->path_at = ft_get_time();
	return (true);
}

/* line of sight through open doors only */
static bool	line_of_sight(t_game *game, t_entity *from, t_entity *to)
{
	t_coords	pos;
	t_raycast	ray;
	t_entity	*ignored;
	double		left;
	int			i;

	pos = from->coords;
	pos.yaw = ft_normalize_angle(ft_degrees(atan2(to->coords.y - pos.y,
					to->coords.x - pos.x)));
	left = ft_distance(from->coords, to->coords);
	ignored = from;
	i = -1;
	while (++i < 8)
	{
		ray = ft_dda_raycast((t_dda_raycast_config){(void ***)game->walls,
				game->map->size, pos, ignored});
		if (!ray.hit || ray.distance >= left)
			return (true);
		if (((t_entity *)ray.hit)->type != ENTITY_DOOR
			|| brain_door_open((t_door *)ray.hit) < 0.5)
			return (false);
		pos.x += ft_cos_degrees(pos.yaw) * ray.distance;
		pos.y += ft_sin_degrees(pos.yaw) * ray.distance;
		left -= ray.distance;
		ignored = ray.hit;
	}
	return (false);
}

bool	brain_can_see(t_game *game, t_character *c, t_character *target)
{
	t_entity	*e;
	double		d;
	double		ang;

	e = (t_entity *)c;
	if (!target || target->dead || !target->billboard.entity.active)
		return (false);
	d = ft_distance(e->coords, target->billboard.entity.coords);
	if (d > AI_VIEW_DISTANCE)
		return (false);
	ang = ft_degrees(atan2(target->billboard.entity.coords.y - e->coords.y,
				target->billboard.entity.coords.x - e->coords.x)) - e->coords.yaw;
	while (ang > 180.0)
		ang -= 360.0;
	while (ang < -180.0)
		ang += 360.0;
	if (fabs(ang) > AI_VIEW_CONE / 2.0 && d > 1.2)
		return (false);
	return (line_of_sight(game, e, (t_entity *)target));
}

t_player	*brain_nearest_player(t_game *game, t_character *c)
{
	t_player	*best;
	double		best_d;
	double		d;
	int			i;

	best = NULL;
	best_d = 1e9;
	i = -1;
	while (++i < PLAYER_MAX)
	{
		if (!game->players[i] || game->players[i]->character.dead)
			continue ;
		d = ft_distance(c->billboard.entity.coords,
				game->players[i]->character.billboard.entity.coords);
		if (d < best_d)
		{
			best_d = d;
			best = game->players[i];
		}
	}
	return (best);
}
