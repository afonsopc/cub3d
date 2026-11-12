/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   story2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 03:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 03:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "story.h"

/* radio line states: 0 waiting for its trigger, 1 queued, 2 said.
** The box leaves the top-right corner free for the touch buttons. */

static bool	sees_type(t_game *game, char c)
{
	t_entity	*e;
	int			i;

	i = -1;
	while (game->billboards && game->billboards[++i])
	{
		e = game->billboards[i];
		if (e->identifier == c && e->character && e->type != ENTITY_PLAYER
			&& !((t_character *)e)->dead && e->controller.brain.sees)
			return (true);
	}
	return (false);
}

static bool	triggered(t_game *game, t_story_radio *r, t_character *p)
{
	t_entity	*pe;
	int			max;

	pe = (t_entity *)p;
	if (r->kind == RADIO_AT)
		return (ft_distance(pe->coords, (t_coords){r->x + 0.5, r->y + 0.5, 0})
			< STORY_TRIGGER_RADIUS);
	if (r->kind == RADIO_SEE)
		return (sees_type(game, r->see));
	if (r->kind == RADIO_GOT)
		return (game_has_key(game, r->see));
	if (r->kind == RADIO_HEALTH)
	{
		max = pe->max_health;
		if (max <= 0)
			max = 100;
		return (!p->dead && pe->health * 100 <= r->health * max);
	}
	return (false);
}

void	story_fire_start_radios(t_game *game)
{
	int	i;

	i = -1;
	while (++i < game->story.radio_count)
		if (game->story.radio[i].kind == RADIO_START
			&& !game->story.radio[i].state)
			game->story.radio[i].state = 1;
}

static long long	radio_length(t_story_radio *r)
{
	return (ft_max(STORY_RADIO_MIN_MS, story_text_len(r->text)
			* STORY_RADIO_PER_CHAR_MS + 2200));
}

void	story_play_frame(t_game *game)
{
	t_story		*st;
	t_character	*p;
	int			i;

	st = &game->story;
#ifdef AI_TRACE
	{
		static t_time	last;

		if (ft_get_time() - last > 5000)
			printf("STORY audio level=%d at %llds\n",
				fta_is_playing(game->background_sound), (long long)(ft_get_time()
					- st->play_start) / 1000);
		if (ft_get_time() - last > 5000)
			last = ft_get_time();
	}
#endif
	if (!game->players[0] || st->phase != STORY_PLAY)
		return ;
	p = (t_character *)game->players[0];
	i = -1;
	while (++i < st->radio_count)
	{
		if (!st->radio[i].state && st->radio[i].unless
			&& game_has_key(game, st->radio[i].unless))
			st->radio[i].state = 3;
		if (!st->radio[i].state && triggered(game, &st->radio[i], p))
			st->radio[i].state = 1;
	}
	if (st->radio_current >= 0 && (long long)ft_get_time()
		- (long long)st->radio_at > radio_length(&st->radio[st->radio_current]))
		st->radio_current = -1;
	i = -1;
	while (st->radio_current < 0 && ++i < st->radio_count)
	{
		if (st->radio[i].state != 1)
			continue ;
		st->radio[i].state = 2;
		st->radio_current = i;
		st->radio_at = ft_get_time();
		printf("STORY radio %d: %s\n", i + 1, st->radio[i].text);
	}
}

void	story_render_radio(t_game *game, t_ftm_image *c)
{
	t_story		*st;
	t_ftm_image	*img;
	t_coords	o;
	t_size		s;

	st = &game->story;
	if (st->radio_current < 0 || st->phase != STORY_PLAY)
		return ;
	o = (t_coords){c->size.width * 0.03, c->size.height * 0.03, 0};
	s = (t_size){c->size.width * 0.80, c->size.height * 0.15};
	ftm_draw_rectangle(c, o, s, (t_ftm_rectangle){0xD8101418, 0xFFE8C860,
		(t_size){2, 2}});
	img = st->radio_portrait ? get_sprite_image(st->radio_portrait) : NULL;
	if (img)
		ftm_put_image_to_canvas(c, img, (t_ftm_pitc_config){.coords = {o.x + 6,
			o.y + 6, 0}, .crop = false, .resize = true, .size = {s.height - 12,
			s.height - 12}, .pixel_modifier = NULL});
	story_text_at(c, game->hud.action_font, (t_ftm_text_config){
		"R\xC1" "DIO - MAJOR ANDRADE", {o.x + s.height + 4, o.y + 6, 0},
		c->size.height * 0.026, 0, 0xFFE8C860}, false);
	story_draw_block(c, game->hud.action_font, st->radio[st->radio_current].text,
		(t_story_box){o.x + s.height + 4, o.y + 8 + c->size.height * 0.035,
		s.width - s.height - 14, s.height, c->size.height * 0.034,
		((long long)ft_get_time() - (long long)st->radio_at)
		* STORY_CHARS_PER_SEC / 1000, 0xFFF2EEDC, false});
}
