/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:11:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 11:44:46 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"

void	call_entity_frames(t_game *game, t_fps *fps)
{
	t_list		*curr;
	t_entity	*entity;

	curr = game->entities;
	while (curr)
	{
		entity = curr->data;
		entity->frame(game, entity, fps->delta_time);
		curr = curr->next;
	}
}

void	call_entity_keys(t_game *game, t_ftm_key_hook_values kvh)
{
	t_list		*curr;
	t_entity	*entity;

	if (!game)
		return ;
	curr = game->entities;
	while (curr)
	{
		entity = curr->data;
		if (entity->active && entity->controller.key)
			entity->controller.key(game, entity, kvh);
		curr = curr->next;
	}
}

bool	entity_x_is_transparent(t_entity *entity, t_direction direction,
	double x)
{
	t_ftm_image		*image;
	unsigned int	*pixel;
	double			actual_x;

	actual_x = x;
	if (direction == NORTH || direction == EAST)
		actual_x = 1.0 - x;
	image = get_sprite_image(get_entity_sprite(entity, direction));
	if (!image)
		return (true);
	pixel = ftm_image_pixel(image, (t_coords){actual_x * image->size.width,
			image->size.height / 2, 0});
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
		fta_play(item->get_sound);
		return (true);
	}
	return (false);
}
