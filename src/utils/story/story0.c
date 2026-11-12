/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   story0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 03:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 03:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "story.h"

/* UTF-8 -> the font's byte encoding (Latin-1 codepoints); '|' stays */
char	*story_text(const char *s)
{
	char			*out;
	size_t			j;
	unsigned int	cp;

	if (!s)
		return (NULL);
	out = ft_calloc(ft_strlen(s) + 1, 1);
	j = 0;
	while (out && *s)
	{
		cp = (unsigned char)*s++;
		if (cp >= 0xC0 && cp < 0xE0 && *s)
			cp = ((cp & 0x1F) << 6) | ((unsigned char)*s++ & 0x3F);
		else if (cp >= 0xE0)
		{
			while ((((unsigned char)*s) & 0xC0) == 0x80)
				s++;
			cp = '-';
		}
		if (cp == 0xB7)
			cp = '-';
		if (cp >= 32 && cp < 255)
			out[j++] = (char)cp;
	}
	return (out);
}

static char	*map_text(t_game *game, char *fmt, int i)
{
	char	*key;
	char	*v;

	key = ft_strf(fmt, i);
	v = ft_hashmap_get_value(game->map->types, key);
	free(key);
	return (story_text(v));
}

static void	*map_sprite(t_game *game, char *fmt, int i)
{
	char	*key;
	void	*v;

	key = ft_strf(fmt, i);
	v = ft_hashmap_get_value(game->sprites, key);
	free(key);
	return (v);
}

/* RADIO_n_GOT c: quando o jogador recebe c; RADIO_n_UNLESS c: não dispara
** se o jogador já tiver c (p.ex. "a porta está trancada" com a chave) */
static void	radio_keys(t_game *game, t_story_radio *r, int n)
{
	char	*key;
	char	*v;

	key = ft_strf("RADIO_%d_GOT", n);
	v = ft_hashmap_get_value(game->map->types, key);
	if (v && *v)
		(r->kind = RADIO_GOT, r->see = *v);
	free(key);
	key = ft_strf("RADIO_%d_UNLESS", n);
	v = ft_hashmap_get_value(game->map->types, key);
	if (v && *v)
		r->unless = *v;
	free(key);
}

static void	load_objectives(t_game *game, t_story *st)
{
	char	*key;
	char	*v;

	st->obj_current = -1;
	while (st->obj_count < STORY_MAX_PANELS)
	{
		st->obj_text[st->obj_count] = map_text(game, "OBJECTIVE_%d_TEXT",
				st->obj_count + 1);
		if (!st->obj_text[st->obj_count])
			return ;
		key = ft_strf("OBJECTIVE_%d_DONE", st->obj_count + 1);
		v = ft_hashmap_get_value(game->map->types, key);
		if (v && *v)
			st->obj_done[st->obj_count] = *v;
		free(key);
		st->obj_count++;
	}
}

static void	load_radios(t_game *game, t_story *st)
{
	t_story_radio	*r;
	char			*key;
	char			*v;

	while (st->radio_count < STORY_MAX_RADIO)
	{
		r = &st->radio[st->radio_count];
		r->text = map_text(game, "RADIO_%d_TEXT", st->radio_count + 1);
		if (!r->text)
			return ;
		key = ft_strf("RADIO_%d_AT", st->radio_count + 1);
		v = ft_hashmap_get_value(game->map->types, key);
		if (v && sscanf(v, "%lf %lf", &r->x, &r->y) == 2)
			r->kind = RADIO_AT;
		free(key);
		key = ft_strf("RADIO_%d_SEE", st->radio_count + 1);
		v = ft_hashmap_get_value(game->map->types, key);
		if (v && *v)
			(r->kind = RADIO_SEE, r->see = *v);
		free(key);
		key = ft_strf("RADIO_%d_HEALTH", st->radio_count + 1);
		v = ft_hashmap_get_value(game->map->types, key);
		if (v && *v)
			(r->kind = RADIO_HEALTH, r->health = ft_atoi(v));
		free(key);
		radio_keys(game, r, st->radio_count + 1);
		st->radio_count++;
	}
}

static void	music(t_game *game, t_fta_audio *play);

/* the intro is shown once per session (dying sends you back to the hub) */
static bool	intro_seen(bool set)
{
	static bool	seen;

	if (set)
		seen = true;
	return (seen);
}

void	story_init(t_game *game)
{
	t_story	*st;

	st = &game->story;
	st->radio_current = -1;
	while (st->intro_count < STORY_MAX_PANELS
		&& (st->intro_text[st->intro_count] = map_text(game,
				"INTRO_%d_TEXT", st->intro_count + 1)))
		st->intro_sprite[st->intro_count++] = map_sprite(game, "INTRO_%d",
				st->intro_count + 1);
	while (st->ending_count < STORY_MAX_PANELS
		&& (st->ending_text[st->ending_count] = map_text(game,
				"ENDING_%d_TEXT", st->ending_count + 1)))
		st->ending_sprite[st->ending_count++] = map_sprite(game,
				"ENDING_%d", st->ending_count + 1);
	st->brief_title = story_text(ft_hashmap_get_value(game->map->types,
				"BRIEFING_TITLE"));
	st->brief_text = story_text(ft_hashmap_get_value(game->map->types,
				"BRIEFING_TEXT"));
	st->level_name = story_text(ft_hashmap_get_value(game->map->types,
				"LEVEL_NAME"));
	st->brief_sprite = ft_hashmap_get_value(game->sprites, "BRIEFING");
	st->stats_sprite = ft_hashmap_get_value(game->sprites, "STATS");
	st->radio_portrait = ft_hashmap_get_value(game->sprites, "RADIO_PORTRAIT");
	st->ending_sound = ft_hashmap_get_value(game->sounds, "ENDING");
	st->intro_sound = ft_hashmap_get_value(game->sounds, "INTRO");
	st->brief_sound = ft_hashmap_get_value(game->sounds, "BRIEFING");
	story_music_init(game);
	load_radios(game, st);
	load_objectives(game, st);
	game->keyring.locked_sound = ft_hashmap_get_value(game->sounds, "LOCKED");
	game->keyring.lever_sound = ft_hashmap_get_value(game->sounds, "LEVER");
	story_count_totals(game);
	st->phase_at = ft_get_time();
	if (st->intro_count && !intro_seen(false))
		(st->phase = STORY_INTRO, music(game, st->intro_sound));
	else if (st->brief_text)
		(st->phase = STORY_BRIEFING, music(game, st->brief_sound));
	else
		story_start_play(game);
}

void	story_free(t_game *game)
{
	t_story	*st;
	int		i;

	st = &game->story;
	i = -1;
	while (++i < STORY_MAX_PANELS)
		(free(st->intro_text[i]), free(st->ending_text[i]));
	i = -1;
	while (++i < st->radio_count)
		free(st->radio[i].text);
	i = -1;
	while (++i < st->obj_count)
		free(st->obj_text[i]);
	(free(st->brief_title), free(st->brief_text), free(st->level_name));
	free(st->pending_map);
	ftm_free_image(st->canvas);
	ft_bzero(st, sizeof(t_story));
}

/* only one music at a time: the level's, or the one of the story screen */
static void	music(t_game *game, t_fta_audio *play)
{
	t_story	*st;

	st = &game->story;
	if (st->playing == play)
		return ;
	fta_stop(st->intro_sound);
	fta_stop(st->brief_sound);
	fta_stop(st->ending_sound);
	fta_stop(ft_hashmap_get_value(game->sounds, "STATS"));
	if (play)
		fta_stop(game->background_sound);
	st->playing = play;
	fta_play(play);
	printf("STORY music %s\n", !play ? "level" : play == st->intro_sound
		? "intro" : play == st->brief_sound ? "briefing" : play
		== st->ending_sound ? "ending" : "stats");
}

bool	story_blocking(t_game *game)
{
	return (game && game->story.phase != STORY_PLAY);
}

void	story_start_play(t_game *game)
{
	t_story	*st;

	st = &game->story;
	music(game, NULL);
	st->phase = STORY_PLAY;
	st->play_start = ft_get_time();
	game->fps.last_frame_time = ft_get_time();
	if (cub3d()->window)
		cub3d()->window->forward_clicks = false;
	story_fire_start_radios(game);
	printf("STORY play %s\n", st->level_name ? st->level_name : "-");
}

static void	finish_level(t_game *game)
{
	t_story	*st;

	st = &game->story;
	music(game, NULL);
	fta_stop(game->background_sound);
	if (cub3d()->window)
		cub3d()->window->forward_clicks = false;
	printf("STORY next map %s\n", st->pending_map);
	cub3d()->new_map_path = st->pending_map;
	st->pending_map = NULL;
	st->phase = STORY_PLAY;
}

static void	enter(t_game *game, int phase)
{
	t_story	*st;

	st = &game->story;
	st->phase = phase;
	st->index = 0;
	st->phase_at = ft_get_time();
	st->typed_all = false;
	fta_stop(game->background_sound);
	if (phase == STORY_ENDING && st->ending_sound)
		music(game, st->ending_sound);
	else if (phase == STORY_STATS && ft_hashmap_get_value(game->sounds,
			"STATS"))
		music(game, ft_hashmap_get_value(game->sounds, "STATS"));
	else if (phase == STORY_BRIEFING || phase == STORY_STATS)
		music(game, st->brief_sound);
	printf("STORY phase %d\n", phase);
}

/* one step forward: next panel, or the next phase */
void	story_next(t_game *game)
{
	t_story	*st;

	st = &game->story;
	st->index++;
	st->phase_at = ft_get_time();
	st->typed_all = false;
	if (st->phase == STORY_INTRO && st->index < st->intro_count)
		return ;
	if (st->phase == STORY_ENDING && st->index < st->ending_count)
		return ;
	if (st->phase == STORY_INTRO)
		intro_seen(true);
	if (st->phase == STORY_INTRO && st->brief_text)
		return (enter(game, STORY_BRIEFING));
	if (st->phase == STORY_INTRO || st->phase == STORY_BRIEFING)
		return (story_start_play(game));
	if (st->phase == STORY_STATS && st->ending_count)
		return (enter(game, STORY_ENDING));
	finish_level(game);
}

/* skip the whole sequence (intro + briefing, or ending) */
void	story_skip(t_game *game)
{
	t_story	*st;

	st = &game->story;
	if (st->phase == STORY_INTRO)
		intro_seen(true);
	if (st->phase == STORY_INTRO || st->phase == STORY_BRIEFING)
		return (story_start_play(game));
	finish_level(game);
}

bool	story_key(t_game *game, t_ftm_key_hook_values khv)
{
	t_story	*st;

	if (!game)
		return (false);
	st = &game->story;
	if (!story_blocking(game))
		return (khv.down && st->play_start
			&& (long long)ft_get_time() - (long long)st->play_start < 700);
	if (!khv.down)
		return (false);
	if (khv.key == FTM_KEY_ESC || (khv.coords.x > 0 && cub3d()->window
			&& khv.coords.x < cub3d()->window->size.width * 0.22
			&& khv.coords.y < cub3d()->window->size.height * 0.12))
		return (story_skip(game), true);
	if ((long long)ft_get_time() - (long long)st->phase_at < 250)
		return (true);
	if (!st->typed_all)
		st->typed_all = true;
	else
		story_next(game);
	return (true);
}

bool	story_level_end(t_game *game, char *next_map)
{
	t_story	*st;

	if (!game || !game->story.level_name)
		return (false);
	st = &game->story;
	if (st->phase != STORY_PLAY || st->pending_map)
		return (true);
	st->pending_map = ft_strdup(next_map);
	story_snapshot_stats(game);
	enter(game, STORY_STATS);
	return (true);
}
