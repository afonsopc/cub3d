/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   story.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 03:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 03:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Story layer in the classic early-90s way: full-screen text panels with a
** typewriter effect (intro, briefing, level stats, ending), and short radio
** lines from the commander at the top of the screen while playing. All the
** text and the radio triggers come from map keys, so they are easy to edit:
**
**   LEVEL_NAME Defesa do Forte
**   INTRO_1_SPRITE assets/oms3d/story/intro-1.bmp     (up to 8 panels)
**   INTRO_1_TEXT linha|outra linha
**   BRIEFING_SPRITE ... / BRIEFING_TITLE ... / BRIEFING_TEXT a|b|c
**   STATS_SPRITE ...
**   ENDING_1_SPRITE / ENDING_1_TEXT ...  ENDING_SOUND assets/...mp3
**   RADIO_PORTRAIT_SPRITE ...
**   RADIO_1_TEXT ...   and one trigger: RADIO_1_AT x y | RADIO_1_SEE e |
**                      RADIO_1_HEALTH 30 | (none = when play starts)
*/

#ifndef STORY_H
# define STORY_H

# include <cub3d.h>
# include <stdio.h>

# define STORY_CHARS_PER_SEC 45.0
# define STORY_RADIO_MIN_MS 3500
# define STORY_RADIO_PER_CHAR_MS 55
# define STORY_TRIGGER_RADIUS 1.1
/* VGA font glyphs are 16x16 boxes: draw them at whole multiples only (crisp
** pixels) and pull the letters together by 6 font pixels */
# define STORY_GLYPH 16
# define STORY_GLYPH_TRIM 6

typedef struct s_story_box
{
	int				x;
	int				y;
	int				w;
	int				h;
	int				text_h;
	int				limit;
	unsigned int	color;
	bool			centered;
}	t_story_box;

/* story0.c */
void		story_start_play(t_game *game);
void		story_next(t_game *game);
void		story_skip(t_game *game);

/* story1.c: drawing */
int			story_draw_block(t_ftm_image *canvas, t_ftm_font *font, char *text,
				t_story_box box);
int			story_visible_chars(t_story *st);
int			story_text_len(char *text);
int			story_snap_h(double h);
int			story_text_w(t_ftm_font *font, char *s, int h);
void		story_draw_panel(t_ftm_image *canvas, t_sprite *sprite);
void		story_text_at(t_ftm_image *canvas, t_ftm_font *font,
				t_ftm_text_config cfg, bool centered);

/* story2.c: radio */
void		story_fire_start_radios(t_game *game);

/* story3.c: stats */
void		story_snapshot_stats(t_game *game);
void		story_count_totals(t_game *game);
void		story_render_stats(t_game *game, t_ftm_image *canvas);

/* story5.c: music the map does not set (stats screen, level playlists) */
void		story_music_init(t_game *game);

#endif
