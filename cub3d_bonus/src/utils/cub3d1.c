/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:13:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 21:13:53 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	free_walls(t_game *game)
{
	int	i;

	if (!game || !game->walls)
		return ;
	i = -1;
	while (++i < game->map->size.height)
		free(game->walls[i]);
	free(game->walls);
	game->walls = NULL;
}

static bool	alloc_walls(t_game *game)
{
	int	i;

	game->walls = ft_calloc(game->map->size.height, sizeof(t_entity **));
	if (!game->walls)
		return (false);
	i = -1;
	while (++i < game->map->size.height)
	{
		game->walls[i] = ft_calloc(game->map->size.width, sizeof(t_entity *));
		if (game->walls[i])
			continue ;
		while (--i >= 0)
		{
			free(game->walls[i]);
			game->walls[i] = NULL;
		}
		free(game->walls);
		game->walls = NULL;
		return (false);
	}
	return (true);
}

void	update_walls_matrix(t_game *game)
{
	t_entity	*entity;
	t_list		*curr;

	if (!game)
		return ;
	if (!game->walls && !alloc_walls(game))
		return ;
	curr = game->entities;
	while (curr)
	{
		entity = curr->data;
		curr = curr->next;
		if (!entity->wall || entity->coords.x < 0
			|| entity->coords.x >= game->map->size.width || entity->coords.y < 0
			|| entity->coords.y >= game->map->size.height)
			continue ;
		game->walls[(int)entity->coords.y][(int)entity->coords.x] = entity;
	}
}

static bool	is_billboard(t_entity *entity, void *param)
{
	(void)param;
	return (entity && entity->billboard);
}

void	update_billboards_vec(t_game *game)
{
	t_list		*curr;
	size_t		i;
	size_t		billboard_count;

	if (!game || !game->entities)
		return ;
	billboard_count = ft_list_count(game->entities, (void *)is_billboard, NULL);
	if (!game->billboards
		|| billboard_count != ft_strvlen((void *)game->billboards))
	{
		free(game->billboards);
		game->billboards = ft_calloc(billboard_count + 1, sizeof(t_entity *));
		if (!game->billboards)
			return ;
	}
	ft_bzero(game->billboards, sizeof(t_entity *) * (billboard_count + 1));
	curr = game->entities;
	i = -1;
	while (curr)
	{
		if (((t_entity *)curr->data)->billboard)
			game->billboards[++i] = curr->data;
		curr = curr->next;
	}
}
