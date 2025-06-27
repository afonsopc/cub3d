/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 23:46:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 17:06:13 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	process_door_at(t_game *game, int y, int x, t_sprite *frame_sprite)
{
	t_door		*door;
	t_entity	*wall1;
	t_entity	*wall2;
	t_direction	dir;

	if (!frame_sprite)
		return ;
	door = (t_door *)game->walls[y][x];
	dir = door->direction;
	wall1 = game->walls[y - (dir == EAST || dir == WEST)][x - (dir == NORTH
			|| dir == SOUTH)];
	wall2 = game->walls[y + (dir == EAST || dir == WEST)][x + (dir == NORTH
			|| dir == SOUTH)];
	if (wall1 && wall1->wall && (dir == EAST || dir == WEST))
		sprite_soft_copy(&((t_wall *)wall1)->south_sprite, frame_sprite);
	else if (wall1 && wall1->wall && (dir == NORTH || dir == SOUTH))
		sprite_soft_copy(&((t_wall *)wall1)->east_sprite, frame_sprite);
	if (wall2 && wall2->wall && (dir == EAST || dir == WEST))
		sprite_soft_copy(&((t_wall *)wall2)->north_sprite, frame_sprite);
	else if (wall2 && wall2->wall && (dir == NORTH || dir == SOUTH))
		sprite_soft_copy(&((t_wall *)wall2)->west_sprite, frame_sprite);
}

void	insert_door_frames(t_game *game)
{
	int	y;
	int	x;

	y = -1;
	while (++y < game->map->size.height)
	{
		x = -1;
		while (++x < game->map->size.width)
			if (game->walls[y][x] && game->walls[y][x]->type == ENTITY_DOOR)
				process_door_at(game, y, x,
					((t_door *)game->walls[y][x])->door_sides_sprite);
	}
}

static int	count_sprite(t_hashmap *sprites, char *key)
{
	int		i;
	char	*full_key;
	void	*found;

	i = 1;
	while (1)
	{
		full_key = ft_strf("%s_%d", key, i);
		found = ft_hashmap_get_value(sprites, full_key);
		free(full_key);
		if (!found)
			break ;
		i++;
	}
	return (i - 1);
}

static void	fill_sprites(t_sprite **sprites, t_hashmap *game_sprites, char *key)
{
	t_ten_ints	ints;
	char		*full_key;

	ints.two = count_sprite(game_sprites, key);
	if (!ints.two)
		return ;
	ints.three = 360 / ints.two;
	ints.five = ints.three / 2;
	ints.one = -1;
	while (++ints.one < 360)
	{
		ints.four = (ft_normalize_angle(ints.one - ints.five)) / ints.three + 1;
		if (ints.four > ints.two)
			ints.four = 1;
		full_key = ft_strf("%s_%d", key, ints.four);
		sprites[ints.one] = ft_hashmap_get_value(game_sprites, full_key);
		free(full_key);
	}
}

void	init_sprites_3d(t_game *game)
{
	t_element	*curr;
	t_element	*el;
	int			i;
	char		*key;
	t_sprite	**sprites;

	el = game->sprites->table;
	while (el)
	{
		curr = el;
		el = el->next;
		i = ft_strlen(curr->key) - 1;
		while (i >= 0 && ft_isdigit(curr->key[i]))
			i--;
		if (i < 1 || curr->key[i] != '_')
			continue ;
		key = ft_strndup(curr->key, i);
		sprites = ft_calloc(360, sizeof(t_sprite *));
		if (!ft_hashmap_get_value(game->sprites_3d, key) && sprites)
			(fill_sprites(sprites, game->sprites, key),
				ft_hashmap_set(game->sprites_3d, key, sprites, free));
		else
			free(sprites);
		free(key);
	}
}
