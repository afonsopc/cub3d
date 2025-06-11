/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities0.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:49:46 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 21:30:14 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"

static t_sprite	*get_wall_sprite(t_wall *wall, t_direction direction)
{
	if (direction == NORTH)
		return (wall->north_sprite);
	else if (direction == SOUTH)
		return (wall->south_sprite);
	else if (direction == WEST)
		return (wall->west_sprite);
	else if (direction == EAST)
		return (wall->east_sprite);
	return (NULL);
}

t_sprite	*get_entity_sprite(t_entity *entity, t_direction direction)
{
	if (!entity)
		return (NULL);
	if (entity->type == ENTITY_WALL || entity->type == ENTITY_DOOR)
		return (get_wall_sprite((t_wall *)entity, direction));
	return (NULL);
}

void	call_entity_frames(t_list *entities, t_fps *fps)
{
	t_list		*curr;
	t_entity	*entity;

	curr = entities;
	while (curr)
	{
		entity = curr->data;
		entity->frame(entity, fps->delta_time);
		curr = curr->next;
	}
}

void	call_entity_keys(t_list *entities, int key, bool down)
{
	t_list		*curr;
	t_entity	*entity;

	curr = entities;
	while (curr)
	{
		entity = curr->data;
		if (entity->active && entity->controller.key)
			entity->controller.key(entity, key, down);
		curr = curr->next;
	}
}

bool	entity_x_is_transparent(t_entity *entity, t_direction direction, double x)
{t_ftm_image		*image;
	unsigned int	*pixel;
	double			actual_x;

	actual_x = x;
	if (direction == NORTH || direction == EAST)
		actual_x = 1.0 - x;
	image = get_sprite_image(get_entity_sprite(entity, direction));
	if (!image)
		return (true);
	pixel = ftm_image_pixel(image, (t_coords){actual_x * image->size.width, image->size.height / 2, 0});
	if (!pixel)
		return (true);
	return (!(*pixel >> 24));
}

void	free_entity(void *data)
{
	if (!data)
		return ;
	((t_entity *)data)->clear(data);
	free(data);
}

t_entity_creator	get_entity_creator(t_identifiers *identifiers, char identifier)
{
	if (ft_list_any(identifiers->air, (void *)ft_str_equal_char_ptr, &identifier))
		return (NULL);
	if (ft_list_any(identifiers->wall, (void *)ft_str_equal_char_ptr, &identifier))
		return ((void *)wall_new);
	if (ft_list_any(identifiers->player, (void *)ft_str_equal_char_ptr, &identifier))
		return ((void *)player_new);
	if (ft_list_any(identifiers->door, (void *)ft_str_equal_char_ptr, &identifier))
		return ((void *)door_new_e);
	if (ft_list_any(identifiers->billboard, (void *)ft_str_equal_char_ptr, &identifier))
		return ((void *)billboard_new);
	if (ft_list_any(identifiers->drop, (void *)ft_str_equal_char_ptr, &identifier))
		return ((void *)drop_new);
	if (ft_list_any(identifiers->character, (void *)ft_str_equal_char_ptr, &identifier))
		return ((void *)character_new);
	return (NULL);
}

bool	add_item_to_inventory(t_character *character, t_item *item)
{
	int	i;

	if (!character || !item)
		return (false);
	i = -1;
	while (++i < INVENTORY_SIZE)
	{
		if (character->inventory[i])
			continue ;
		character->inventory[i] = item;
		return (true);
	}
	return (false);
}

void	fill_death_sprites(t_sprite **dst, t_sprite *src)
{
    int	i;

    i = -1;
    while (++i < 360)
    {
        if (src)
        {
            dst[i] = ft_calloc(1, sizeof(t_sprite));
            if (dst[i])
            {
                *dst[i] = *src;
                dst[i]->index = 0;
                dst[i]->running = false;
            }
        }
    }
}

void	fill_3d_sprites_from_src(t_sprite **dst, t_sprite **src)
{
	int	i;

	i = -1;
	while (++i < 360)
		dst[i] = src[i];
}

void	fill_3d_sprites_from_single(t_sprite **dst, t_sprite *src)
{
	int	i;

	i = -1;
	while (++i < 360)
		dst[i] = src;
}

void	fill_3d_sprites_from_game(t_game *game, t_sprite **dst, char identifier, char *key)
{
    t_sprite	**sprites;
    t_sprite	*sprite;

    sprites = hashmap_get_with_identifier(game, game->sprites_3d, identifier, key);
    if (sprites)
        return (fill_3d_sprites_from_src(dst, sprites));
    sprite = hashmap_get_with_identifier(game, game->sprites, identifier, key);
    if (key && ft_strequal(key, "DEATH"))
        return (fill_death_sprites(dst, sprite));
    fill_3d_sprites_from_single(dst, sprite);
}
