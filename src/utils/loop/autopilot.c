/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autopilot.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 18:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 18:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Piloto automático de teste (só nos builds com -DAI_TRACE, tecla F10).
** Joga o nível como um jogador: segue o objetivo atual (a chave, a alavanca,
** os documentos e por fim a alavanca da saída), anda com as teclas de
** movimento e as colisões normais, abre portas com a tecla de usar, apanha
** socorros e munições quando precisa e dispara contra quem vê. Não faz
** batota: vida, dano e munições são os do jogo. Escreve "AUTO ..." na
** consola para o teste (tests/levels/run.mjs) seguir o percurso.
*/

#ifdef AI_TRACE

# include "loop.h"
# include "../entities/controller/enemy/brain.h"

typedef struct s_auto
{
	bool		on;
	t_game		*game;
	int			*prev;
	int			*queue;
	int			cells;
	t_coords	last_pos;
	t_time		moved_at;
	t_time		unstick_until;
	int			unstick_dir;
	t_time		pulse_at;
	t_entity	*goal;
	t_entity	*fighting;
	t_time		aim_since;
	t_time		action_at;
}	t_auto;

static t_auto	*ap(void)
{
	static t_auto	a;

	return (&a);
}

void	autopilot_toggle(t_game *game)
{
	ap()->on = !ap()->on || ap()->game != game;
	ap()->game = game;
	printf("AUTO %s\n", ap()->on ? "on" : "off");
}

/* ------------------------------------------------------------ caminhos */

static bool	walkable(t_game *g, int x, int y)
{
	t_entity	*e;

	if (x < 0 || y < 0 || x >= g->map->size.width || y >= g->map->size.height)
		return (false);
	e = g->walls[y][x];
	if (e && e->type == ENTITY_DOOR && !((t_door *)e)->closeable)
		return (((t_door *)e)->opened);
	return (brain_cell_walkable(g, x, y, true));
}

/* BFS a partir do jogador; devolve a distância até (gx, gy) ou -1 */
static int	bfs(t_game *g, int px, int py, int goal)
{
	t_auto	*a;
	int		head;
	int		tail;
	int		k;
	int		n[4];

	a = ap();
	if (a->cells != g->map->size.width * g->map->size.height)
	{
		free(a->prev);
		free(a->queue);
		a->cells = g->map->size.width * g->map->size.height;
		a->prev = malloc(sizeof(int) * a->cells);
		a->queue = malloc(sizeof(int) * a->cells);
	}
	k = -1;
	while (++k < a->cells)
		a->prev[k] = -2;
	head = 0;
	tail = 0;
	a->queue[tail++] = py * g->map->size.width + px;
	a->prev[py * g->map->size.width + px] = -1;
	while (head < tail)
	{
		k = a->queue[head++];
		if (k == goal)
			break ;
		n[0] = k + 1;
		n[1] = k - 1;
		n[2] = k + g->map->size.width;
		n[3] = k - g->map->size.width;
		for (int i = 0; i < 4; i++)
		{
			if (n[i] < 0 || n[i] >= a->cells || a->prev[n[i]] != -2)
				continue ;
			if ((i < 2) && n[i] / g->map->size.width != k / g->map->size.width)
				continue ;
			if (n[i] != goal && !walkable(g, n[i] % g->map->size.width,
					n[i] / g->map->size.width))
				continue ;
			a->prev[n[i]] = k;
			a->queue[tail++] = n[i];
		}
	}
	if (a->prev[goal] == -2)
		return (-1);
	k = goal;
	head = 0;
	while (a->prev[k] >= 0)
		(k = a->prev[k], head++);
	return (head);
}

/* primeiro passo do caminho até goal (BFS já feito) */
static int	first_step(int goal)
{
	int	k;

	k = goal;
	while (ap()->prev[k] >= 0 && ap()->prev[ap()->prev[k]] >= 0)
		k = ap()->prev[k];
	return (k);
}

/* ----------------------------------------------------------- objetivos */

static int	cell_of(t_game *g, t_entity *e)
{
	return ((int)e->coords.y * g->map->size.width + (int)e->coords.x);
}

/* uma alavanca é uma parede: o alvo é a casa de chão ao lado mais perto */
static int	lever_cell(t_game *g, t_entity *lever, int px, int py, int *dist)
{
	int	best;
	int	d;
	int	c;
	int	dx[4];

	dx[0] = 1;
	dx[1] = -1;
	dx[2] = g->map->size.width;
	dx[3] = -g->map->size.width;
	best = -1;
	*dist = 1 << 30;
	for (int i = 0; i < 4; i++)
	{
		c = cell_of(g, lever) + dx[i];
		if (!walkable(g, c % g->map->size.width, c / g->map->size.width))
			continue ;
		d = bfs(g, px, py, c);
		if (d >= 0 && d < *dist)
			(*dist = d, best = c);
	}
	return (best);
}

static bool	has_item(t_character *p, char id)
{
	int	i;

	i = -1;
	while (++i < INVENTORY_SIZE)
		if (p->inventory[i] && p->inventory[i]->identifier == id)
			return (true);
	return (false);
}

static bool	wanted_drop(t_game *g, t_character *p, t_drop *d, char key)
{
	t_collectible	*c;

	if (!d->billboard.entity.active || !d->item)
		return (false);
	if (d->item->weapon)
		return (!key && !has_item(p, d->item->identifier));
	if (!d->item->collectible)
		return (false);
	c = (t_collectible *)d->item;
	if (key)
		return (c->gives == key);
	if (c->health && p->billboard.entity.health * 100
		< p->billboard.entity.max_health * 55)
		return (true);
	(void)g;
	return (c->ammo && p->ammo < 12);
}

static int	near_supply(t_game *g, t_character *p, int px, int py, int *dist)
{
	t_list	*l;
	t_drop	*d;
	int		best;
	int		dd;

	best = -1;
	*dist = 1 << 30;
	l = g->entities;
	while (l)
	{
		d = l->data;
		l = l->next;
		if (d->billboard.entity.type != ENTITY_DROP
			|| !wanted_drop(g, p, d, 0))
			continue ;
		dd = bfs(g, px, py, cell_of(g, (t_entity *)d));
		if (dd >= 0 && dd < *dist && dd < 14)
			(*dist = dd, best = cell_of(g, (t_entity *)d));
	}
	return (best);
}

static int	objective_cell(t_game *g, t_character *p, int px, int py)
{
	t_list		*l;
	t_entity	*e;
	char		key;
	int			dist;
	int			best;

	key = 0;
	if (g->story.obj_current >= 0 && g->story.obj_current < g->story.obj_count)
		key = g->story.obj_done[g->story.obj_current];
	best = -1;
	l = g->entities;
	while (l && best < 0)
	{
		e = l->data;
		l = l->next;
		if (key && e->type == ENTITY_DROP
			&& wanted_drop(g, p, (t_drop *)e, key))
			(ap()->goal = e, best = cell_of(g, e));
		else if (e->type == ENTITY_ELEVATOR && ((!key && ((t_elevator *)e)
				->map_path && e->identifier == 'n') || (key && ((t_elevator *)e)
				->gives == key && !((t_elevator *)e)->pulled)))
			(ap()->goal = e, best = lever_cell(g, e, px, py, &dist));
	}
	return (best);
}

/* ----------------------------------------------------------- comandos */

static void	stop(t_controller *c)
{
	c->walking_forward = false;
	c->walking_backward = false;
	c->walking_left = false;
	c->walking_right = false;
	c->sprinting = false;
	c->action = false;
}

static double	turn_to(t_entity *e, t_coords t, double max)
{
	double	want;
	double	diff;

	want = ft_degrees(atan2(t.y - e->coords.y, t.x - e->coords.x));
	diff = want - e->coords.yaw;
	while (diff > 180.0)
		diff -= 360.0;
	while (diff < -180.0)
		diff += 360.0;
	if (diff > max)
		diff = max;
	if (diff < -max)
		diff = -max;
	e->coords.yaw = ft_normalize_angle(e->coords.yaw + diff);
	want = ft_degrees(atan2(t.y - e->coords.y, t.x - e->coords.x))
		- e->coords.yaw;
	while (want > 180.0)
		want -= 360.0;
	while (want < -180.0)
		want += 360.0;
	return (fabs(want));
}

static void	pick_weapon(t_character *p)
{
	const char	*order = "GMPK";
	int			i;
	int			j;

	j = -1;
	while (order[++j])
	{
		i = -1;
		while (++i < INVENTORY_SIZE)
			if (p->inventory[i] && p->inventory[i]->identifier == order[j]
				&& (order[j] == 'K' || p->ammo > 0))
				return ((void)(p->inventory_index = i));
	}
}

static void	trigger(t_character *p, bool down)
{
	t_item	*it;

	it = p->inventory[p->inventory_index];
	if (it)
		it->user = NULL;
	if (it && down)
		it->user = p;
}

static t_character	*visible_enemy(t_game *g, t_character *p)
{
	t_character	*best;
	t_entity	*e;
	double		bd;
	double		d;
	int			i;

	best = NULL;
	bd = 13.0;
	i = -1;
	while (g->billboards && g->billboards[++i])
	{
		e = g->billboards[i];
		if (!e->character || e->type == ENTITY_PLAYER || !e->active
			|| ((t_character *)e)->dead)
			continue ;
		d = ft_distance(e->coords, p->billboard.entity.coords);
		if (d < bd && brain_line_of_sight(g, (t_entity *)p, e))
			(bd = d, best = (t_character *)e);
	}
	return (best);
}

static bool	fight(t_game *g, t_character *p, double dt)
{
	t_character	*en;
	t_entity	*pe;
	double		off;
	bool		down;

	pe = (t_entity *)p;
	en = visible_enemy(g, p);
	if (!en)
		return (trigger(p, false), ap()->fighting = NULL, false);
	if ((t_entity *)en != ap()->fighting)
	{
		printf("AUTO fight %c at %.1f %.1f\n", en->billboard.entity.identifier,
			en->billboard.entity.coords.x, en->billboard.entity.coords.y);
		ap()->fighting = (t_entity *)en;
		ap()->aim_since = ft_get_time();
	}
	if (p->target_entity == (t_entity *)en)
		ap()->aim_since = ft_get_time();
	if ((long long)ft_get_time() - (long long)ap()->aim_since > 2500)
		return (trigger(p, false), false);
	stop(&pe->controller);
	pick_weapon(p);
	off = turn_to(pe, en->billboard.entity.coords, 720.0 * dt);
	down = off < 6.0 && p->target_entity == (t_entity *)en
		&& (ft_get_time() / 90) % 2;
	trigger(p, down);
	if (p->inventory[p->inventory_index]
		&& p->inventory[p->inventory_index]->identifier == 'K'
		&& ft_distance(pe->coords, en->billboard.entity.coords) > 0.9)
		pe->controller.walking_forward = off < 20.0;
	return (true);
}

static void	unstick(t_entity *pe)
{
	t_auto	*a;

	a = ap();
	if (ft_distance(pe->coords, a->last_pos) > 0.08)
	{
		a->last_pos = pe->coords;
		a->moved_at = ft_get_time();
		return ;
	}
	if ((long long)ft_get_time() - (long long)a->moved_at < 900)
		return ;
	a->unstick_dir = !a->unstick_dir;
	a->unstick_until = ft_get_time() + 350;
	a->moved_at = ft_get_time();
	printf("AUTO unstick at %.1f %.1f\n", pe->coords.x, pe->coords.y);
}

/* a tecla de usar fica em baixo uma imagem a cada 400 ms */
static void	press_action(t_entity *pe)
{
	if ((long long)ft_get_time() - (long long)ap()->action_at < 400)
		return ;
	pe->controller.action = true;
	ap()->action_at = ft_get_time();
}

static void	walk_to(t_game *g, t_entity *pe, int step, bool arrived)
{
	t_coords	t;
	t_entity	*w;
	double		off;

	if (arrived && ap()->goal && ap()->goal->type == ENTITY_ELEVATOR)
	{
		t = (t_coords){ap()->goal->coords.x + 0.5, ap()->goal->coords.y + 0.5,
			0};
		if (turn_to(pe, t, 12.0) < 3.0)
			press_action(pe);
		return ;
	}
	t = (t_coords){step % g->map->size.width + 0.5, step
		/ g->map->size.width + 0.5, 0};
	w = g->walls[step / g->map->size.width][step % g->map->size.width];
	off = turn_to(pe, t, 12.0);
	if (w && w->type == ENTITY_DOOR && w->hard && !((t_door *)w)->opened
		&& off < 10.0)
		press_action(pe);
	pe->controller.walking_forward = off < 50.0;
	pe->controller.sprinting = true;
	if ((long long)ft_get_time() < (long long)ap()->unstick_until)
	{
		pe->controller.walking_left = ap()->unstick_dir;
		pe->controller.walking_right = !ap()->unstick_dir;
	}
}

void	autopilot_frame(t_game *g, t_entity *pe)
{
	t_character	*p;
	int			px;
	int			py;
	int			goal;
	int			d;

	if (!ap()->on || ap()->game != g || pe != (t_entity *)g->players[0]
		|| g->story.phase != STORY_PLAY || ((t_character *)pe)->dead)
		return ;
	p = (t_character *)pe;
	stop(&pe->controller);
	if (fight(g, p, g->fps.delta_time))
		return ;
	px = (int)pe->coords.x;
	py = (int)pe->coords.y;
	ap()->goal = NULL;
	goal = near_supply(g, p, px, py, &d);
	if (goal < 0)
		goal = objective_cell(g, p, px, py);
	if (goal < 0 || bfs(g, px, py, goal) < 0)
	{
		if ((ft_get_time() / 1000) % 3 == 0 && ft_get_time() % 1000 < 20)
			printf("AUTO no path from %d %d\n", px, py);
		return ;
	}
	unstick(pe);
	d = first_step(goal);
	if ((long long)ft_get_time() - (long long)ap()->moved_at > 600
		&& (long long)ft_get_time() - (long long)ap()->moved_at < 640)
		printf("AUTO stuck at %.2f %.2f yaw %.0f step %d %d goal %d %d\n",
			pe->coords.x, pe->coords.y, pe->coords.yaw, d % g->map->size.width,
			d / g->map->size.width, goal % g->map->size.width,
			goal / g->map->size.width);
	walk_to(g, pe, d, goal == py * g->map->size.width + px);
}

#endif
