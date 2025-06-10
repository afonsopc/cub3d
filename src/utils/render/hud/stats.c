/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:14:35 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 16:43:15 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hud.h"

void	render_ammo_text(t_game *game, t_ftm_image *canvas)
{
	t_coords			text_pos;
	char				*ammo_text;
	double				ammo_scaler;

	ammo_text = ft_strf("%d", game->player->character.billboard.entity.ammo);
	ammo_scaler = 1.47;
	if (ft_strlen(ammo_text) == 3)
		ammo_scaler = 1.50;
	if (ft_strlen(ammo_text) == 2)
		ammo_scaler = 1.48;
	text_pos = (t_coords){canvas->size.width / ammo_scaler,
		canvas->size.height / 1.09, 0};
	ftm_draw_text(canvas, game->hud.stats.font,
		(t_ftm_text_config){
			.text = ammo_text,
			.coords = text_pos,
			.height = canvas->size.height * 0.05,
			.spacing = 4,
			.color = 0xCFFFFFFF
		});
	free(ammo_text);
}

void	render_health_text(t_game *game, t_ftm_image *canvas)
{
	t_coords			text_pos;
	char				*health_text;
	double				health_scaler;

	health_text = ft_strf("%d%%", game->player->character.billboard.entity.health);
	health_scaler = 1.84;
	if (ft_strlen(health_text) == 3)
		health_scaler = 1.81;
	if (ft_strlen(health_text) == 2)
		health_scaler = 1.70;
	text_pos = (t_coords){canvas->size.width / health_scaler,
		canvas->size.height / 1.09, 0};
	ftm_draw_text(canvas, game->hud.stats.font,
		(t_ftm_text_config){
			.text = health_text,
			.coords = text_pos,
			.height = canvas->size.height * 0.05,
			.spacing = 4,
			.color = 0xCFFFFFFF
		});
	free(health_text);
}

static t_size	get_hand_item_size(t_ftm_image *image, double max_w, double max_h)
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

void	render_hand_item_icon(t_game *game, t_ftm_image *canvas)
{
    t_item		*item;
    t_entity	*entity;
    t_ftm_image	*image;
    t_size		item_size;

    entity = (t_entity *)game->player;
    if (!entity)
        return ;
    item = entity->inventory[entity->inventory_index];
    if (!item)
        return ;
    image = get_sprite_image(item->icon_sprite);
	item_size = get_hand_item_size(image, canvas->size.width * 0.15, canvas->size.height * 0.10);
    ftm_put_image_to_canvas(canvas, image,
        (t_ftm_pitc_config){
        .coords = (t_coords){(canvas->size.width * 0.87) - (item_size.width / 2),
            (canvas->size.height * 0.92) - (item_size.height / 2), 0},
        .crop = false,
        .resize = true,
        .size = item_size,
        .pixel_modifier = NULL
    });
}

void	render_stats(t_game *game, t_ftm_image *canvas)
{
	t_size		stats_size;
	t_ftm_image	*image;
	int			index;

	if (!game->hud.stats.states)
		return ;
	stats_size = (t_size){canvas->size.width, canvas->size.width / 8};
	index = (int)(game->hud.stats.states
			* ((float)game->player->character.billboard.entity.health
				/ game->player->character.billboard.entity.max_health));
	index = game->hud.stats.states - index;
	if (index >= game->hud.stats.states)
		index = game->hud.stats.states - 1;
	if (index < 0)
		index = 0;
	image = get_sprite_image(ft_list_index(game->hud.stats.states_list,
				index)->data);
	ftm_put_image_to_canvas(canvas, image,
		(t_ftm_pitc_config){
		(t_coords){0, canvas->size.height - stats_size.height, 0},
		false, (t_coords){0, 0, 0}, (t_coords){0, 0, 0},
		true, stats_size, NULL, NULL});
	render_health_text(game, canvas);
	render_ammo_text(game, canvas);
	render_hand_item_icon(game, canvas);
}
