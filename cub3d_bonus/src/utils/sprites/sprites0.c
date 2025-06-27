/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:01:44 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 00:56:07 by afpachec         ###   ########.fr       */
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
