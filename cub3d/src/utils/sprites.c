/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:01:44 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/14 15:22:36 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	clear_sprite(void *data)
{
	ft_list_destroy(&((t_sprite *)data)->images);
}

void	free_sprite(void *data)
{
	clear_sprite(data);
	free(data);
}

void	init_sprite(t_sprite *sprite, t_list *images, t_time update_delay)
{
	sprite->images = images;
	sprite->update_delay = update_delay;
	sprite->updated_at = ft_get_time();
}

t_sprite	*sprite_new(t_list *images, t_time update_delay)
{
	t_sprite	*sprite;

	sprite = ft_calloc(1, sizeof(t_sprite));
	if (!sprite)
		return (NULL);
	init_sprite(sprite, images, update_delay);
	return (sprite);
}

t_ftm_image	*get_sprite_image(t_sprite *sprite)
{
	t_list	*curr_image_node;

	if (!sprite)
		return (NULL);
	if (ft_get_time() - sprite->updated_at > sprite->update_delay)
	{
		sprite->index++;
		if (sprite->index >= ft_list_size(sprite->images))
			sprite->index = 0;
		sprite->updated_at = ft_get_time();
	}
	curr_image_node = ft_list_index(sprite->images, sprite->index);
	if (!curr_image_node)
		return (NULL);
	return (curr_image_node->data);
}
