/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:48:25 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/27 19:01:41 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minimap.h"

void	set_entity_color(t_minimap_data mp_data, t_entity *entity,
	unsigned int *color)
{
	if (entity->type == ENTITY_WALL)
		*color = mp_data.game->hud.minimap.wall_color;
	else if (entity->type == ENTITY_PLAYER)
		*color = mp_data.game->hud.minimap.player_color;
	else if (entity->type == ENTITY_DOOR)
		*color = mp_data.game->hud.minimap.door_color;
	else if (entity->type == ENTITY_ELEVATOR)
		*color = mp_data.game->hud.minimap.elevator_color;
	else
		*color = mp_data.game->hud.minimap.entity_color;
}

bool	in_boundaries(t_minimap_data mp_data, t_coords entity_pos)
{
	return (entity_pos.x + mp_data.size.width * 0.2 >= mp_data.coords.x
		&& entity_pos.y + mp_data.size.height * 0.2 >= mp_data.coords.y
		&& entity_pos.x - mp_data.size.width * 0.2 < mp_data.coords.x
		+ mp_data.size.width && entity_pos.y - mp_data.size.height * 0.2
		< mp_data.coords.y + mp_data.size.height);
}

void	fix_clipping(t_minimap_data mp_data, t_coords *pos, t_size *size)
{
	int	clip_left;
	int	clip_right;
	int	clip_top;
	int	clip_bottom;

	clip_left = mp_data.coords.x - pos->x;
	clip_right = (pos->x + size->width) - (mp_data.coords.x
			+ mp_data.size.width);
	clip_top = mp_data.coords.y - pos->y;
	clip_bottom = (pos->y + size->height) - (mp_data.coords.y
			+ mp_data.size.height);
	if (clip_left > 0)
	{
		pos->x += clip_left;
		size->width -= clip_left;
	}
	if (clip_right > 0)
		size->width -= clip_right;
	if (clip_top > 0)
	{
		pos->y += clip_top;
		size->height -= clip_top;
	}
	if (clip_bottom > 0)
		size->height -= clip_bottom;
}

void	render_entity(t_minimap_data mp_data, t_entity *entity,
	t_coords coords, t_size entity_size)
{
	unsigned int	color;
	t_coords		new_coords;
	t_size			draw_size;

	new_coords = (t_coords){(int)(coords.x + (entity->coords.x
				* entity_size.width)), (int)(coords.y + (entity->coords.y
				* entity_size.height)), entity->coords.yaw};
	if (!in_boundaries(mp_data, new_coords))
		return ;
	set_entity_color(mp_data, entity, &color);
	if (entity == (t_entity *)mp_data.character)
	{
		ftm_draw_arrow(mp_data.canvas,
			new_coords,
			(t_size){entity_size.width / 1.5, entity_size.height / 1.5},
			color);
		return ;
	}
	draw_size = (t_size){entity_size.width * entity->size.width,
		entity_size.height * entity->size.height};
	fix_clipping(mp_data, &new_coords, &draw_size);
	ftm_draw_rectangle(mp_data.canvas,
		new_coords, draw_size, (t_ftm_rectangle){color, color, (t_size){1, 1}});
}

void	calculate_minimap_scale(t_minimap_data mp_data, t_dsize *scale,
	t_size *entity_size, t_coords *entity_offset)
{
	t_coords	player_center;

	scale->width = fmin((double)mp_data.size.width
			/ (double)mp_data.game->map->size.width,
			(double)mp_data.size.height
			/ (double)mp_data.game->map->size.height);
	scale->height = scale->width * mp_data.game->hud.minimap.zoom_level;
	*entity_size = (t_size){scale->height, scale->height};
	player_center = (t_coords){
		mp_data.character->billboard.entity.coords.x,
		mp_data.character->billboard.entity.coords.y,
		0};
	*entity_offset = (t_coords){(mp_data.size.width / 2)
		- (player_center.x * scale->height), (mp_data.size.height / 2)
		- (player_center.y * scale->height), 0};
}
