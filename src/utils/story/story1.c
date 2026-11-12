/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   story1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 03:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 03:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "story.h"

int	story_text_len(char *text)
{
	int	n;

	n = 0;
	while (text && *text)
		n += (*text++ != '|');
	return (n);
}

/* nearest whole multiple of the glyph size (never below 1x) */
int	story_snap_h(double h)
{
	int	k;

	k = (int)(h / STORY_GLYPH + 0.5);
	if (k < 1)
		k = 1;
	return (k * STORY_GLYPH);
}

int	story_visible_chars(t_story *st)
{
	return ((int)(((long long)ft_get_time() - (long long)st->phase_at)
		* STORY_CHARS_PER_SEC / 1000.0));
}

void	story_text_at(t_ftm_image *canvas, t_ftm_font *font,
	t_ftm_text_config cfg, bool centered)
{
	unsigned int	color;

	cfg.height = story_snap_h(cfg.height);
	cfg.spacing = -STORY_GLYPH_TRIM * (cfg.height / STORY_GLYPH);
	if (centered)
		cfg.coords.x -= ftm_get_text_size_prediction(font, cfg) / 2;
	cfg.coords.x = (int)cfg.coords.x;
	cfg.coords.y = (int)cfg.coords.y;
	color = cfg.color;
	cfg.color = 0xFF000000;
	cfg.coords.x += 1;
	cfg.coords.y += 1;
	ftm_draw_text(canvas, font, cfg);
	cfg.coords.x -= 1;
	cfg.coords.y -= 1;
	cfg.color = color;
	ftm_draw_text(canvas, font, cfg);
}

int	story_text_w(t_ftm_font *font, char *s, int h)
{
	h = story_snap_h(h);
	return (ftm_get_text_size_prediction(font, (t_ftm_text_config){s,
			{0, 0, 0}, h, -STORY_GLYPH_TRIM * (h / STORY_GLYPH), 0}));
}

/* draws one wrapped line (up to `left` visible chars); returns chars used */
static int	draw_line(t_ftm_image *canvas, t_ftm_font *font, char *line,
	t_story_box *b)
{
	int		n;
	char	saved;

	n = ft_strlen(line);
	if (n > b->limit)
		n = b->limit;
	saved = line[n];
	line[n] = '\0';
	story_text_at(canvas, font, (t_ftm_text_config){line, {b->x + (b->centered
				* b->w / 2.0), b->y, 0}, b->text_h, 0, b->color}, b->centered);
	line[n] = saved;
	b->limit -= n;
	b->y += b->text_h * 1.45;
	return (n);
}

/* word wrap one paragraph into the box width */
static void	draw_paragraph(t_ftm_image *canvas, t_ftm_font *font, char *p,
	t_story_box *b)
{
	char	buf[256];
	char	tmp[256];
	int		len;
	int		word;
	int		t;

	len = 0;
	while (*p && b->limit > 0)
	{
		word = 0;
		while (p[word] && p[word] != ' ' && word < 200)
			word++;
		ft_memcpy(tmp, buf, len);
		t = len;
		if (t)
			tmp[t++] = ' ';
		ft_memcpy(tmp + t, p, word);
		t += word;
		tmp[t] = '\0';
		if (len && (t > 250 || story_text_w(font, tmp, b->text_h) > b->w))
		{
			buf[len] = '\0';
			draw_line(canvas, font, buf, b);
			len = 0;
			continue ;
		}
		ft_memcpy(buf, tmp, t + 1);
		len = t;
		p += word;
		while (*p == ' ')
			p++;
	}
	if (len && b->limit > 0)
		draw_line(canvas, font, buf, b);
}

/* draws text with '|' as line breaks; returns the number of text lines */
int	story_draw_block(t_ftm_image *canvas, t_ftm_font *font, char *text,
	t_story_box b)
{
	char	*copy;
	char	*p;
	char	*bar;
	int		y0;

	if (!text || !font)
		return (0);
	b.text_h = story_snap_h(b.text_h);
	copy = ft_strdup(text);
	p = copy;
	y0 = b.y;
	while (p && *p && b.limit > 0)
	{
		bar = ft_strchr(p, '|');
		if (bar)
			*bar = '\0';
		draw_paragraph(canvas, font, p, &b);
		if (!bar)
			break ;
		p = bar + 1;
	}
	free(copy);
	return ((b.y - y0) / (b.text_h * 1.45));
}

void	story_draw_panel(t_ftm_image *canvas, t_sprite *sprite)
{
	t_ftm_image	*img;

	ftm_draw_rectangle(canvas, (t_coords){0, 0, 0}, canvas->size,
		(t_ftm_rectangle){0xFF0A0A0E, 0, (t_size){0, 0}});
	img = sprite ? get_sprite_image(sprite) : NULL;
	if (!img)
		return ;
	ftm_put_image_to_canvas(canvas, img, (t_ftm_pitc_config){.coords = {0, 0,
		0}, .crop = false, .resize = true, .size = canvas->size,
		.pixel_modifier = NULL});
}

/* intro/briefing: SALTAR in the top band and the hint at the bottom;
** stats/ending: CONTINUAR at the bottom right */
static void	draw_hints(t_game *game, t_ftm_image *c, bool done, bool skip)
{
	t_ftm_font	*f;
	int			h;
	char		*t;

	f = game->hud.action_font;
	h = STORY_GLYPH;
	if (skip)
		story_text_at(c, f, (t_ftm_text_config){"SALTAR >>", {c->size.width
				* 0.025, c->size.height * 0.018, 0}, h, 0, 0xFFB8B8B0}, false);
	if (!done || !((ft_get_time() / 450) % 2))
		return ;
	t = "Toque ou carregue numa tecla";
	if (!skip)
		t = "CONTINUAR >";
	if (skip)
		story_text_at(c, f, (t_ftm_text_config){t, {c->size.width / 2,
				c->size.height * 0.955, 0}, h, 0, 0xFFE8C860}, true);
	else
		story_text_at(c, f, (t_ftm_text_config){t, {c->size.width * 0.97
				- story_text_w(f, t, h * 2), c->size.height * 0.93, 0}, h * 2,
			0, 0xFFE8C860}, false);
}

static void	render_panel_text(t_game *game, t_ftm_image *c, char *title,
	char *text)
{
	t_story		*st;
	int			total;
	t_story_box	b;

	st = &game->story;
	total = story_text_len(text);
	if (story_visible_chars(st) >= total)
		st->typed_all = true;
	b = (t_story_box){c->size.width * 0.07, c->size.height * 0.70,
		c->size.width * 0.86, c->size.height * 0.24, c->size.height * 0.042,
		st->typed_all ? 100000 : story_visible_chars(st), 0xFFF2EEDC, false};
	if (title)
	{
		story_text_at(c, game->hud.action_font, (t_ftm_text_config){title,
			{c->size.width / 2, c->size.height * 0.08, 0},
			c->size.height * 0.06, 0, 0xFFE8C860}, true);
		b.y = c->size.height * 0.62;
	}
	story_draw_block(c, game->hud.action_font, text, b);
	draw_hints(game, c, st->typed_all, st->phase != STORY_ENDING);
}

void	story_frame(t_game *game, t_ftm_window *window)
{
	t_story		*st;
	t_ftm_image	*c;

	st = &game->story;
	window->forward_clicks = true;
	if (!st->canvas || st->canvas->size.width != window->size.width
		|| st->canvas->size.height != window->size.height)
		(ftm_free_image(st->canvas), st->canvas = ftm_image_new(window,
				window->size));
	c = st->canvas;
	if (!c)
		return ;
	if (st->phase == STORY_INTRO)
		(story_draw_panel(c, st->intro_sprite[st->index]),
			render_panel_text(game, c, NULL, st->intro_text[st->index]));
	else if (st->phase == STORY_BRIEFING)
		(story_draw_panel(c, st->brief_sprite),
			render_panel_text(game, c, st->brief_title, st->brief_text));
	else if (st->phase == STORY_ENDING)
		(story_draw_panel(c, st->ending_sprite[st->index]),
			render_panel_text(game, c, NULL, st->ending_text[st->index]));
	else if (st->phase == STORY_STATS)
		(story_draw_panel(c, st->stats_sprite), story_render_stats(game, c),
			draw_hints(game, c, st->typed_all, false));
	ftm_put_image_to_window(window, c, (t_coords){0, 0, 0});
}
