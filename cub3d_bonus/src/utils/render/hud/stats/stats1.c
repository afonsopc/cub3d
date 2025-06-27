/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:56:01 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 19:57:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stats.h"

void	render_score_text(t_game *game, t_ftm_image *canvas,
	t_character *character, t_size stats_size)
{
	t_coords	text_pos;
	char		*score_text;
	double		score_scaler;

	score_text = ft_strf("%d", character->score);
	score_scaler = 4.85;
	if (character->score > 99999)
		character->score = 99999;
	if (ft_strlen(score_text) == 3)
		score_scaler = 5.25;
	if (ft_strlen(score_text) == 4)
		score_scaler = 5.45;
	if (ft_strlen(score_text) == 5)
		score_scaler = 5.65;
	text_pos = (t_coords){stats_size.width / score_scaler,
		canvas->size.height - stats_size.height * 0.5, 0};
	ftm_draw_text(canvas, game->hud.stats_font,
		(t_ftm_text_config){
		.text = score_text,
		.coords = text_pos,
		.height = stats_size.width * 0.04,
		.spacing = 4,
		.color = 0xCFFFFFFF
	});
	free(score_text);
}

void	render_ammo_text(t_game *game, t_ftm_image *canvas,
	t_character *character, t_size stats_size)
{
	t_coords	text_pos;
	char		*ammo_text;
	double		ammo_scaler;

	if (character->ammo > 999)
		ammo_text = ft_strdup("999");
	else
		ammo_text = ft_strf("%d", character->ammo);
	ammo_scaler = 1.47;
	if (ft_strlen(ammo_text) == 3)
		ammo_scaler = 1.50;
	if (ft_strlen(ammo_text) == 2)
		ammo_scaler = 1.48;
	text_pos = (t_coords){stats_size.width / ammo_scaler,
		canvas->size.height - stats_size.height * 0.5, 0};
	ftm_draw_text(canvas, game->hud.stats_font,
		(t_ftm_text_config){
		.text = ammo_text,
		.coords = text_pos,
		.height = stats_size.width * 0.04,
		.spacing = 4,
		.color = 0xCFFFFFFF
	});
	free(ammo_text);
}

void	render_health_text(t_game *game, t_ftm_image *canvas,
	t_character *character, t_size stats_size)
{
	t_coords			text_pos;
	char				*health_text;
	double				health_scaler;

	health_text = ft_strf("%d%%", character->billboard.entity.health);
	health_scaler = 1.88;
	if (ft_strlen(health_text) == 3)
		health_scaler = 1.81;
	if (ft_strlen(health_text) == 2)
		health_scaler = 1.78;
	text_pos = (t_coords){stats_size.width / health_scaler,
		canvas->size.height - stats_size.height * 0.5, 0};
	ftm_draw_text(canvas, game->hud.stats_font,
		(t_ftm_text_config){
		.text = health_text,
		.coords = text_pos,
		.height = stats_size.width * 0.04,
		.spacing = 4,
		.color = 0xCFFFFFFF
	});
	free(health_text);
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
	item_size = get_hand_item_size(image, stats_size.width * 0.15,
			stats_size.height * 0.5);
	ftm_put_image_to_canvas(canvas, image,
		(t_ftm_pitc_config){
		.coords = (t_coords){(stats_size.width * 0.87) - (item_size.width / 2),
		(canvas->size.height - stats_size.height * 0.5) - (item_size.height
			/ 2), 0},
		.crop = false,
		.resize = true,
		.size = item_size,
		.pixel_modifier = NULL
	});
}
