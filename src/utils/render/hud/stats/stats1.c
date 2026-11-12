/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:56:01 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/03 17:25:39 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stats.h"

/*
** The status bar art (scripts/oms_art/hud.py) is 640 units wide; each value
** is drawn centred in its own box, below the label, and shrunk when the
** text would not fit (big scores, health on maps with a huge MAX_HEALTH).
*/
static void	draw_in_box(t_game *game, t_ftm_image *canvas, char *text,
	t_hud_box box)
{
	t_ftm_text_config	cfg;
	double				u;
	int					w;
	int					maxw;

	u = box.stats_size.width / 640.0;
	cfg = (t_ftm_text_config){text, (t_coords){0, 0, 0},
		box.stats_size.width * 0.04, box.stats_size.width * 0.008,
		0xCFFFFFFF};
	maxw = (box.x1 - box.x0 - 10) * u;
	w = ftm_get_text_size_prediction(game->hud.stats_font, cfg);
	while (w > maxw && cfg.height > 6)
	{
		cfg.height = cfg.height * 0.9;
		cfg.spacing = cfg.height / 5;
		w = ftm_get_text_size_prediction(game->hud.stats_font, cfg);
	}
	cfg.coords.x = (box.x0 + box.x1) / 2.0 * u - (w - cfg.spacing) / 2.0;
	cfg.coords.y = canvas->size.height - box.stats_size.height
		+ box.stats_size.height * (49.0 / 80.0) - cfg.height / 2.0;
	ftm_draw_text(canvas, game->hud.stats_font, cfg);
}

void	render_score_text(t_game *game, t_ftm_image *canvas,
	t_character *character, t_size stats_size)
{
	char	*text;

	if (character->score > 99999)
		character->score = 99999;
	text = ft_strf("%d", character->score);
	draw_in_box(game, canvas, text, (t_hud_box){84, 196, stats_size});
	free(text);
}

void	render_ammo_text(t_game *game, t_ftm_image *canvas,
	t_character *character, t_size stats_size)
{
	char	*text;

	if (character->ammo > 999)
		text = ft_strdup("999");
	else
		text = ft_strf("%d", character->ammo);
	draw_in_box(game, canvas, text, (t_hud_box){410, 480, stats_size});
	free(text);
}

/* health as a percentage of the maximum, like the classic status bars */
void	render_health_text(t_game *game, t_ftm_image *canvas,
	t_character *character, t_size stats_size)
{
	char	*text;
	int		hp;
	int		max;

	hp = character->billboard.entity.health;
	max = character->billboard.entity.max_health;
	if (max > 0 && max != 100)
		hp = (int)((long long)hp * 100 / max + (hp > 0 && hp * 100LL < max));
	text = ft_strf("%d%%", hp);
	draw_in_box(game, canvas, text, (t_hud_box){334, 406, stats_size});
	free(text);
}

t_size	get_hand_item_size(t_ftm_image *image, double max_w,
	double max_h)
{
	t_size		item_size;
	double		aspect_ratio;

	aspect_ratio = (double)image->size.width / image->size.height;
	if (aspect_ratio > 1.0)
	{
		item_size.width = max_w;
		item_size.height = max_w / aspect_ratio;
		if (item_size.height > max_h)
		{
			item_size.height = max_h;
			item_size.width = max_h * aspect_ratio;
		}
		return (item_size);
	}
	item_size.height = max_h;
	item_size.width = max_h * aspect_ratio;
	if (item_size.width > max_w)
	{
		item_size.width = max_w;
		item_size.height = max_w / aspect_ratio;
	}
	return (item_size);
}

void	render_hand_item_icon(t_ftm_image *canvas, t_character *character,
	t_size stats_size)
{
	t_item		*item;
	t_ftm_image	*image;
	t_size		item_size;

	item = character->inventory[character->inventory_index];
	if (!item)
		return ;
	image = get_sprite_image(item->icon_sprite);
	item_size = get_hand_item_size(image, stats_size.width * (96.0 / 640.0),
			stats_size.height * 0.5);
	ftm_put_image_to_canvas(canvas, image,
		(t_ftm_pitc_config){
		.coords = (t_coords){(stats_size.width * (581.0 / 640.0))
		- (item_size.width / 2),
		(canvas->size.height - stats_size.height * 0.5) - (item_size.height
			/ 2), 0},
		.crop = false,
		.resize = true,
		.size = item_size,
		.pixel_modifier = NULL
	});
}
