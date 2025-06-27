/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hand_item.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:28:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 19:52:36 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hud.h"

static t_size	get_hand_item_size(t_size max_size, t_ftm_image	*image)
{
	double		aspect_ratio;
	double		width_ratio;
	double		height_ratio;

	aspect_ratio = (double)image->size.width / image->size.height;
	width_ratio = max_size.width / image->size.width;
	height_ratio = max_size.height / image->size.height;
	if (width_ratio < height_ratio)
		return ((t_size){max_size.width, max_size.width / aspect_ratio});
	else
		return ((t_size){max_size.height * aspect_ratio, max_size.height});
}

void	render_hand_item(t_ftm_image *canvas, t_character *character,
	t_size stats_size)
{
	t_sprite	*sprite;
	t_ftm_image	*image;
	t_size		display_size;
	t_size		max_size;

	if (!character->inventory[character->inventory_index])
		return ;
	sprite = character->inventory[character->inventory_index]->screen_sprite;
	if (!sprite)
		sprite = character->inventory[character->inventory_index]->icon_sprite;
	image = get_sprite_image(sprite);
	max_size = (t_size){canvas->size.width, canvas->size.height
		- stats_size.height};
	display_size = get_hand_item_size(max_size, image);
	ftm_put_image_to_canvas(canvas, image,
		(t_ftm_pitc_config){
		.coords = (t_coords){(max_size.width - display_size.width) / 2,
		(max_size.height - display_size.height), 0},
		.crop = false,
		.resize = true,
		.size = display_size,
		.pixel_modifier = NULL
	});
}
