/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:10:50 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 16:10:58 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"

void	clear_3d_sprite(t_sprite **sprites)
{
	int	i;

	if (!sprites)
		return ;
	i = -1;
	while (++i < 360)
	{
		if (!sprites[i])
			continue ;
		free(sprites[i]);
		sprites[i] = NULL;
	}
}

void	fill_3d_sprites_from_src(t_sprite **dst, t_sprite **src)
{
	int	i;

	i = -1;
	clear_3d_sprite(dst);
	while (++i < 360)
		sprite_soft_copy(&(dst[i]), src[i]);
}

void	fill_3d_sprites_from_single(t_sprite **dst, t_sprite *src)
{
	int	i;

	i = -1;
	clear_3d_sprite(dst);
	while (++i < 360)
		sprite_soft_copy(&(dst[i]), src);
}

void	fill_3d_sprites_from_game(t_game *game, t_sprite **dst, char identifier,
	char *key)
{
	t_sprite	**sprites;
	t_sprite	*sprite;

	sprites = hashmap_get_with_identifier(game, game->sprites_3d, identifier,
			key);
	if (sprites)
		return (fill_3d_sprites_from_src(dst, sprites));
	sprite = hashmap_get_with_identifier(game, game->sprites, identifier, key);
	fill_3d_sprites_from_single(dst, sprite);
}

void	reset_3d_sprites(t_sprite **sprites)
{
	int	i;

	if (!sprites)
		return ;
	i = -1;
	while (++i < 360)
	{
		if (!sprites[i])
			continue ;
		sprites[i]->index = 0;
		sprites[i]->updated_at = ft_get_time();
		sprites[i]->running = true;
		sprites[i]->reversed = false;
	}
}
