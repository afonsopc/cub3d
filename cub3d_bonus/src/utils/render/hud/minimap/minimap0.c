/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:48:25 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/26 02:39:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minimap.h"

static void	render_entities(t_minimap_data mp_data)
{
	t_list		*entity_item;
	t_entity	*entity;
	t_size		entity_size;
	t_dsize		scale;
	t_coords	entity_offset;

	calculate_minimap_scale(mp_data, &scale, &entity_size, &entity_offset);
	entity_item = mp_data.game->entities;
	while (entity_item)
	{
		entity = entity_item->data;
		if (entity != (t_entity *)mp_data.character && entity->active)
			render_entity(mp_data, entity,
				(t_coords){
				mp_data.coords.x + entity_offset.x,
				mp_data.coords.y + entity_offset.y, 0},
				entity_size);
		entity_item = entity_item->next;
	}
	render_entity(mp_data, (t_entity *)mp_data.character,
		(t_coords){mp_data.coords.x + entity_offset.x,
		mp_data.coords.y + entity_offset.y, 0}, entity_size);
}

static void	render_minimapp(t_minimap_data mp_data)
{
	ftm_draw_rectangle(mp_data.canvas, mp_data.coords, mp_data.size,
		(t_ftm_rectangle){mp_data.game->hud.minimap.background_color,
		mp_data.game->hud.minimap.border_color, (t_size){1, 1}});
	render_entities(mp_data);
}

void	render_minimap(t_game *game, t_ftm_image *canvas,
	t_character *character)
{
	t_minimap_data	mp_data;

	mp_data = (t_minimap_data){game, canvas, character, {0, 0, 0},
		canvas->size};
	if (!game->hud.minimap.full)
	{
		mp_data.size = (t_size){canvas->size.width * MINIMAP_WIDTH_MULTIPLIER,
			canvas->size.height * MINIMAP_HEIGHT_MULTIPLIER};
		mp_data.coords = (t_coords){canvas->size.width - mp_data.size.width
			- 10, 10, 0};
	}
	render_minimapp(mp_data);
}
