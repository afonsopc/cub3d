/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   story3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 03:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 03:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "story.h"
#include <stdarg.h>

static char	*sfmt(const char *f, ...)
{
	va_list	ap;
	char	*out;

	out = malloc(64);
	if (!out)
		return (NULL);
	va_start(ap, f);
	vsnprintf(out, 64, f, ap);
	va_end(ap);
	return (out);
}

static bool	is_treasure(t_entity *e)
{
	t_drop	*d;

	if (e->type != ENTITY_DROP)
		return (false);
	d = (t_drop *)e;
	return (d->item && d->item->collectible
		&& ((t_collectible *)d->item)->score > 0);
}

/* counts[0] enemies, [1] dead, [2] treasure left, [3] secrets, [4] opened */
static void	count(t_game *game, int counts[5])
{
	t_list		*l;
	t_entity	*e;

	ft_bzero(counts, sizeof(int) * 5);
	l = game->entities;
	while (l)
	{
		e = l->data;
		l = l->next;
		if (e->character && e->type != ENTITY_PLAYER)
		{
			counts[0]++;
			counts[1] += ((t_character *)e)->dead;
		}
		if (is_treasure(e) && e->active)
			counts[2]++;
		if (e->type == ENTITY_DOOR && !((t_door *)e)->closeable)
		{
			counts[3]++;
			counts[4] += ((t_door *)e)->opened;
		}
	}
}

void	story_count_totals(t_game *game)
{
	int	c[5];

	count(game, c);
	game->story.kills_total = c[0];
	game->story.treasure_total = c[2];
	game->story.secrets_total = c[3];
}

void	story_snapshot_stats(t_game *game)
{
	t_story	*st;
	int		c[5];

	st = &game->story;
	count(game, c);
	st->kills = c[1];
	st->treasure = st->treasure_total - c[2];
	st->secrets = c[4];
	st->seconds = ((long long)ft_get_time() - (long long)st->play_start) / 1000;
	st->score = 0;
	if (game->players[0])
		st->score = game->players[0]->character.score;
	printf("STORY stats kills %d/%d treasure %d/%d secrets %d/%d time %d\n",
		st->kills, st->kills_total, st->treasure, st->treasure_total,
		st->secrets, st->secrets_total, st->seconds);
}

static int	pct(int a, int b)
{
	if (b <= 0)
		return (100);
	return (a * 100 / b);
}

static void	row(t_game *game, t_ftm_image *c, int i, char *label, char *value)
{
	int	h;
	int	y;

	h = c->size.height * 0.05;
	y = c->size.height * (0.36 + 0.09 * i);
	story_text_at(c, game->hud.action_font, (t_ftm_text_config){label,
		{c->size.width * 0.18, y, 0}, h, 0, 0xFFE8C860}, false);
	story_text_at(c, game->hud.action_font, (t_ftm_text_config){value,
		{c->size.width * 0.58, y, 0}, h, 0, 0xFFF2EEDC}, false);
	free(value);
}

void	story_render_stats(t_game *game, t_ftm_image *c)
{
	t_story	*st;
	char	*title;
	int		shown;

	st = &game->story;
	shown = ((long long)ft_get_time() - (long long)st->phase_at) / 450;
	if (shown >= 6)
		st->typed_all = true;
	if (st->typed_all)
		shown = 6;
	title = story_text("N\xC3\x8DVEL CONCLU\xC3\x8D" "DO");
	story_text_at(c, game->hud.action_font, (t_ftm_text_config){title,
		{c->size.width / 2, c->size.height * 0.08, 0}, c->size.height * 0.065,
		0, 0xFFE8C860}, true);
	free(title);
	story_text_at(c, game->hud.action_font, (t_ftm_text_config){
		st->level_name, {c->size.width / 2, c->size.height * 0.19, 0},
		c->size.height * 0.045, 0, 0xFFF2EEDC}, true);
	if (shown > 0)
		row(game, c, 0, "INIMIGOS", sfmt("%d/%d  %d%%", st->kills,
				st->kills_total, pct(st->kills, st->kills_total)));
	if (shown > 1)
		row(game, c, 1, "TESOURO", sfmt("%d/%d  %d%%", st->treasure,
				st->treasure_total, pct(st->treasure, st->treasure_total)));
	if (shown > 2)
		row(game, c, 2, "SEGREDOS", sfmt("%d/%d  %d%%", st->secrets,
				st->secrets_total, pct(st->secrets, st->secrets_total)));
	if (shown > 3)
		row(game, c, 3, "TEMPO", sfmt("%02d:%02d", st->seconds / 60,
				st->seconds % 60));
	if (shown > 4)
		row(game, c, 4, "PONTOS", sfmt("%d", st->score));
}
