/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:20:04 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/22 11:56:36 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static int	is_valid_sprite_file(char *value)
{
	return (ft_strequal(value, "NULL") || (ft_str_endswith(value, ".xpm")
			&& ft_is_file(value)));
}

static t_ftm_image	*load_sprite_image(t_ftm_window *window, char *value)
{
	if (ft_strequal(value, "NULL"))
		return (NULL);
	return (ftm_image_from_file(window, value));
}

static int	create_sprite_if_needed(t_game *game, char *key)
{
	t_sprite	*sprite;

	if (ft_hashmap_get_value(game->sprites, key))
		return (1);
	sprite = sprite_new(NULL, 0);
	if (!sprite)
		return (0);
	ft_hashmap_set(game->sprites, key, sprite, free_sprite);
	return (1);
}

static void	add_image_to_sprite(t_game *game, char *key, t_ftm_image *image)
{
	t_sprite	*sprite;

	if (!image)
		return ;
	sprite = (t_sprite *)ft_hashmap_get_value(game->sprites, key);
	ft_list_add(&sprite->images, image, ftm_free_image);
}

void	init_sprites_e(t_ftm_window *window, t_game *game)
{
	t_ftm_image	*image;
	t_element	*el;

	fte_set(ERROR_NO_ERROR);
	el = *game->map->types->table;
	while (el)
	{
		if (is_valid_sprite_file(el->value))
		{
			image = load_sprite_image(window, el->value);
			if (!image && !ft_strequal(el->value, "NULL"))
				return (fte_set(ERROR_LOAD_SPRITE));
			if (!create_sprite_if_needed(game, el->key))
				return (fte_set(ERROR_LOAD_SPRITE));
			add_image_to_sprite(game, el->key, image);
		}
		el = el->next;
	}
}
