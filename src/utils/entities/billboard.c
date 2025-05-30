/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   billboard.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/22 14:37:18 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"

static	int count_sprite(t_hashmap *sprites, char identifier)
{
	int	i;

	i = 1;
	while (hashmap_get_with_identifier(sprites, identifier, ft_strf("%d_SPRITE", i)))
		++i;
	return (i - 1);

}

static void	fill_sprites(t_billboard *billboard, t_hashmap *game_sprites, char identifier)
{
	int	x;
	int	y;
	int	count;
	int	size;

	count = count_sprite(game_sprites, identifier);
	if (!count)
		return ;
	size = 360 / count;
	x = 0;
	while (x < count * size)
	{
		y = -1;
		while (++y < size)
			billboard->sprites[x + y] = hashmap_get_with_identifier(game_sprites, identifier, ft_strf("%d_SPRITE", (x / size) + 1));
		x += size;
	}
}

void	init_billboard(t_game *game, t_billboard *billboard, char identifier)
{
	int	i;

	i = -1;
	init_entity(game, (t_entity *)billboard, identifier);
	billboard->entity.type = ENTITY_BILLBOARD;
	billboard->entity.billboard = true;
	if (hashmap_get_with_identifier(game->sprites, billboard->entity.identifier, "SPRITE"))
	{
		while (++i < 360)
			billboard->sprites[i] = hashmap_get_with_identifier(game->sprites, billboard->entity.identifier, "SPRITE");
		return ;
	}
	fill_sprites(billboard, game->sprites, billboard->entity.identifier);
}


t_billboard	*billboard_new(t_game *game, t_ftm_window *window, char identifier)
{
	t_billboard	*billboard;

	(void)window;
	billboard = ft_calloc(1, sizeof(t_billboard));
	if (!billboard)
		return (NULL);
	init_billboard(game, billboard, identifier);
	billboard->entity.free = free;
	return (billboard);
}
