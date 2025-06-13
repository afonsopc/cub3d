/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:01:44 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/12 18:28:27 by afpachec         ###   ########.fr       */
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

void	sprite_soft_copy(t_sprite **dst, t_sprite *src)
{
	if (!src || !dst)
		return ;
	if (!*dst)
		*dst = ft_calloc(1, sizeof(t_sprite));
	if (!*dst)
		return ;
	**dst = *src;
	(*dst)->index = 0;
	(*dst)->running = true;
	(*dst)->updated_at = ft_get_time();
}

t_sprite	*sprite_new(t_list *images, t_time update_delay)
{
	t_sprite	*sprite;

	sprite = ft_calloc(1, sizeof(t_sprite));
	if (!sprite)
		return (NULL);
	sprite->running = true;
	init_sprite(sprite, images, update_delay);
	return (sprite);
}

t_ftm_image	*get_sprite_image(t_sprite *sprite)
{
	t_list	*curr_image_node;
	ssize_t	new_index;
	t_time	update_diference;

	if (!sprite)
		return (get_sprite_image(&cub3d()->placeholder));
	update_diference = ft_get_time() - sprite->updated_at;
	if (sprite->update_delay && sprite->running
		&& update_diference > sprite->update_delay)
	{
		int delta = (int)(update_diference / sprite->update_delay);
		ssize_t count = (ssize_t)ft_list_size(sprite->images);
		if (sprite->reversed)
			new_index = sprite->index - delta;
		else
			new_index = sprite->index + delta;
		if (sprite->loop)
			new_index = ((new_index % count) + count) % count;
		else if (new_index < 0 || new_index >= count)
		{
			sprite->running = false;
			new_index = (count - 1) * (new_index >= count);
		}
		sprite->index = new_index;
		sprite->updated_at = ft_get_time();
	}
	curr_image_node = ft_list_index(sprite->images, sprite->index);
	if (!curr_image_node)
		return (NULL);
	return (curr_image_node->data);
}
