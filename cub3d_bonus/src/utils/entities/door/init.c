/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 18:51:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "door.h"

static void	init_animation_sprites_e(t_door *door, t_ftm_window *window)
{
	t_ftm_image			*frame;
	t_ftm_image			*door_image;
	t_ftm_pitc_config	config;
	int					i;

	fte_set(NULL);
	ft_bzero(&config, sizeof(t_ftm_pitc_config));
	door_image = get_sprite_image(door->door_sprite);
	door->opening_sprite = sprite_new(NULL, door->animation_delay);
	door->animation_frames = door->animation_delay * DOOR_ANIMATION_FPS;
	i = -1;
	while (++i < door->animation_frames - 1)
	{
		frame = ftm_image_new(window, door_image->size);
		if (!frame)
			return (fte_set("init door frame"));
		config.coords = (t_coords){(int)((double)door_image->size.width
				/ door->animation_frames * i), 0, 0};
		ftm_put_image_to_canvas(frame, door_image, config);
		ft_list_add(&door->opening_sprite->images, frame, ftm_free_image);
	}
	ft_list_add(&door->opening_sprite->images, NULL, NULL);
	door->opening_sprite->running = false;
}

static void	set_sprites(t_door *door)
{
	if (door->direction == NORTH || door->direction == SOUTH)
	{
		door->wall.north_sprite = door->opening_sprite;
		door->wall.west_sprite = door->door_sides_sprite;
	}
	else if (door->direction == EAST || door->direction == WEST)
	{
		door->wall.west_sprite = door->opening_sprite;
		door->wall.north_sprite = door->door_sides_sprite;
	}
	door->wall.east_sprite = door->wall.west_sprite;
	door->wall.south_sprite = door->wall.north_sprite;
}

static void	crazy_door_things(t_game *game, t_ftm_window *window,
	t_door *door, char identifier)
{
	door->auto_close_delay = fabs(ft_atof(hashmap_get_with_identifier(game,
					game->map->types, identifier, "AUTO_CLOSE_DELAY")));
	door->direction = ft_direction_from_str(hashmap_get_with_identifier(game,
				game->map->types, identifier, "DIRECTION"));
	if (door->direction == (t_direction)-1)
		return (fte_set("door direction missing"));
	sprite_soft_copy(&door->door_sprite, hashmap_get_with_identifier(game,
			game->sprites, identifier, "DOOR"));
	sprite_soft_copy(&door->door_sides_sprite, hashmap_get_with_identifier(game,
			game->sprites, identifier, "SIDES"));
	if (!door->door_sprite)
		return (fte_set("door sprite missing"));
	door->open_sound = hashmap_get_with_identifier(game, game->sounds,
			identifier, "OPEN");
	door->close_sound = hashmap_get_with_identifier(game, game->sounds,
			identifier, "CLOSE");
	door->closeable = !ft_strequal(hashmap_get_with_identifier(game,
				game->map->types, identifier, "CLOSEABLE"), "FALSE");
	door->animation_delay = ft_atof(hashmap_get_with_identifier(game,
				game->map->types, identifier, "ANIMATION_DELAY")) * 10;
	init_animation_sprites_e(door, window);
	set_sprites(door);
}

void	init_door_e(t_game *game, t_ftm_window *window, t_door *door,
	char identifier)
{
	fte_set(NULL);
	init_wall(game, window, &door->wall, identifier);
	door->wall.entity.type = ENTITY_DOOR;
	door->wall.entity.frame = door_frame;
	door->wall.entity.clear = clear_door;
	door->wall.entity.action = door_action;
	door->wall.entity.shot = door_shot;
	door->wall.entity.actionable = true;
	crazy_door_things(game, window, door, identifier);
}

t_door	*door_new_e(t_game *game, t_ftm_window *window, char identifier)
{
	t_door	*door;

	fte_set(NULL);
	door = ft_calloc(1, sizeof(t_door));
	if (!door)
		return (NULL);
	init_door_e(game, window, door, identifier);
	if (fte_flagged())
		return (free_entity(door), NULL);
	return (door);
}
