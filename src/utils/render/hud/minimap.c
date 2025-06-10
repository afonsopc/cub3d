/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:48:25 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/10 16:43:21 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hud.h"

static void	set_entity_color(t_game *game, t_entity *entity, unsigned *color)
{
	if (entity->type == ENTITY_WALL)
		*color = game->hud.minimap.wall_color;
	else if (entity->type == ENTITY_PLAYER)
		*color = game->hud.minimap.player_color;
	else if (entity->type == ENTITY_DOOR)
		*color = game->hud.minimap.door_color;
	else
		*color = game->hud.minimap.entity_color;
}

static bool in_boundaries(t_coords entity_pos, t_coords minimap_pos, t_size minimap_size)
{
    return (entity_pos.x + minimap_size.width * 0.2>= minimap_pos.x &&
            entity_pos.y + minimap_size.height * 0.2 >= minimap_pos.y &&
            entity_pos.x - minimap_size.width * 0.2 < minimap_pos.x + minimap_size.width &&
            entity_pos.y - minimap_size.height * 0.2 < minimap_pos.y + minimap_size.height);
}

static void fix_clipping(t_coords *pos, t_size *size, t_coords minimap_pos, t_size minimap_size)
{
	int	clip_left;
	int	clip_right;
	int	clip_top;
	int	clip_bottom;
	
	clip_left = minimap_pos.x - pos->x;
	clip_right = (pos->x + size->width) - (minimap_pos.x + minimap_size.width);
	clip_top = minimap_pos.y - pos->y;
	clip_bottom = (pos->y + size->height) - (minimap_pos.y + minimap_size.height);
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

static void	render_entity(t_ftm_image *canvas, t_game *game, t_entity *entity, t_coords minimap_coords, t_size minimap_size, t_coords coords, t_size entity_size)
{
	unsigned int	color;
	t_coords		new_coords;
    t_size          draw_size;

	new_coords = (t_coords){(int)(coords.x + (entity->coords.x * entity_size.width)), (int)(coords.y + (entity->coords.y * entity_size.height)), entity->coords.yaw};
    if (!in_boundaries(new_coords, minimap_coords, minimap_size))
        return ;
    set_entity_color(game, entity, &color);
	if (entity->type == ENTITY_PLAYER)
	{
		ftm_draw_arrow(canvas,
				new_coords,
				(t_size){entity_size.width / 1.5, entity_size.height / 1.5},
				color);
		return ;
	}
    draw_size = (t_size){entity_size.width * entity->size.width,
    entity_size.height * entity->size.height};
    fix_clipping(&new_coords, &draw_size, minimap_coords, minimap_size);
	ftm_draw_rectangle(canvas,
			new_coords,
			draw_size,
			(t_ftm_rectangle){color, color, (t_size){1, 1}});
}

static void render_entities(t_ftm_image *canvas, t_game *game, t_coords minimap_coords, t_size minimap_size)
{
    t_list      *entity_item;
    t_entity    *entity;
    t_size      entity_size;
    double      scale;
    double      original_scale;
    t_coords    player_center;
    t_coords    entity_offset;

    original_scale = fmin((double)minimap_size.width / (double)game->map->size.width,
                (double)minimap_size.height / (double)game->map->size.height);
    scale = original_scale * game->hud.minimap.zoom_level;
    entity_size = (t_size){scale, scale};
    player_center = (t_coords){
        game->player->character.billboard.entity.coords.x,
        game->player->character.billboard.entity.coords.y,
        0};
    entity_offset = (t_coords){
        (minimap_size.width / 2) - (player_center.x * scale),
        (minimap_size.height / 2) - (player_center.y * scale), 0};
    entity_item = game->entities;
    while (entity_item)
    {
        entity = entity_item->data;
        if (entity != (t_entity *)game->player)
            render_entity(canvas, game, entity, minimap_coords, minimap_size,
                    (t_coords){
                        minimap_coords.x + entity_offset.x,
                        minimap_coords.y + entity_offset.y, 0},
                    entity_size);
        entity_item = entity_item->next;
    }
    render_entity(canvas, game, (t_entity *)game->player, minimap_coords, minimap_size,
        (t_coords){
            minimap_coords.x + entity_offset.x,
            minimap_coords.y + entity_offset.y,0},
        entity_size);
}

static void	render_minimapp(t_game *game, t_ftm_image *canvas, t_coords coords, t_size size)
{
	ftm_draw_rectangle(canvas, coords, size, (t_ftm_rectangle){game->hud.minimap.background_color, game->hud.minimap.border_color, (t_size){1, 1}});
	render_entities(canvas, game, coords, size);
}

void	render_minimap(t_game *game, t_ftm_image *canvas)
{
	t_coords	map_coords;
	t_size		map_size;

	map_coords = (t_coords){0, 0, 0};
	map_size = canvas->size;
	if (!game->hud.minimap.full)
	{
		map_size = (t_size){canvas->size.width * MINIMAP_WIDTH_MULTIPLIER, canvas->size.height * MINIMAP_HEIGHT_MULTIPLIER};
		map_coords = (t_coords){canvas->size.width - map_size.width - 10, 10, 0};
	}
	render_minimapp(game, canvas, map_coords, map_size);
}
