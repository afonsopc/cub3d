/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hand_item.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:28:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 18:57:58 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hud.h"

t_ftm_pitc_config	get_pitc_config(t_ftm_image *canvas, t_ftm_image *image)
{
	t_size	new_size;
	static double angle;

	angle += 1.0;
	if (angle > 360)
		angle = 0.0;
	new_size = (t_size){
		.width = (canvas->size.width / 1.5),
		.height = (canvas->size.height / 1.5) * ((double)image->size.width / image->size.height)
	};
	return ((t_ftm_pitc_config){
		.coords = (t_coords){(canvas->size.width / 2) - (new_size.width / 2),
			(canvas->size.height / 2) - (new_size.height / 2), 0},
		.crop = false,
		.resize = true,
		.size = new_size,
		.pixel_modifier = NULL
	});
}

void	render_hand_item(t_game *game, t_ftm_image *canvas)
{
	t_character	*character;
	t_item		*item;
	t_sprite	*sprite;
	t_ftm_image	*image;

	character = (t_character *)game->player;
	if (!character)
		return ;
	item = character->inventory[character->inventory_index];
	if (!item)
		return ;
	sprite = item->screen_sprite;
	if (!sprite)
		sprite = item->icon_sprite;
	image = get_sprite_image(sprite);
	ftm_put_image_to_canvas(canvas, image, get_pitc_config(canvas, image));
}
