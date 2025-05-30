/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:52:20 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/23 17:23:49 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

static bool	identifier_already_defined(t_identifiers *identifiers, char *identifier)
{
	return (ft_list_any(identifiers->wall, (void *)ft_strequal, identifier)
		|| ft_list_any(identifiers->player, (void *)ft_strequal, identifier)
		|| ft_list_any(identifiers->door, (void *)ft_strequal, identifier)
		|| ft_list_any(identifiers->air, (void *)ft_strequal, identifier)
		|| ft_list_any(identifiers->enemy, (void *)ft_strequal, identifier)
		|| ft_list_any(identifiers->billboard, (void *)ft_strequal, identifier));
}

static void	add_identifier_e(t_identifiers *identifiers, char *identifier, char *type)
{
	fte_set(ERROR_NO_ERROR);
	if (ft_strequal(type, "WALL"))
		ft_list_add(&identifiers->wall, ft_strdup(identifier), free);
	else if (ft_strequal(type, "PLAYER"))
		ft_list_add(&identifiers->player, ft_strdup(identifier), free);
	else if (ft_strequal(type, "DOOR"))
		ft_list_add(&identifiers->door, ft_strdup(identifier), free);
	else if (ft_strequal(type, "ENEMY"))
		ft_list_add(&identifiers->enemy, ft_strdup(identifier), free);
	else if (ft_strequal(type, "BILLBOARD"))
		ft_list_add(&identifiers->billboard, ft_strdup(identifier), free);
	else
		fte_set(ERROR_INVALID_IDENTIFIER_TYPE);
}

static void add_default_identifiers(t_identifiers *identifiers)
{
	int	i;

	i = -1;
	while (DEFAULT_AIR_TYPES[++i])
		ft_list_add(&identifiers->air, ft_strndup(&DEFAULT_AIR_TYPES[i], 1), free);
	i = -1;
	while (DEFAULT_WALL_TYPES[++i])
		ft_list_add(&identifiers->wall, ft_strndup(&DEFAULT_WALL_TYPES[i], 1), free);
	i = -1;
	while (DEFAULT_PLAYER_TYPES[++i])
		ft_list_add(&identifiers->player, ft_strndup(&DEFAULT_PLAYER_TYPES[i], 1), free);
}

void	parse_identifiers_e(t_map *map)
{
	char		*identifier;
	t_element	*el;
	t_element	*curr;

	add_default_identifiers(&map->identifiers);
	el = *map->types->table;
	while (el)
	{
		curr = el;
		el = el->next;
		if (!ft_str_endswith(curr->key, "_TYPE"))
			continue ;
		identifier = ft_strndup(curr->key, ft_strlen(curr->key) - 5);
		if (!identifier)
			return (fte_set(ERROR_EMPTY_TYPE_IDENTIFIER));
		if (identifier_already_defined(&map->identifiers, identifier))
			return (fte_set(ERROR_DUPLICATE_TYPE_IDENTIFIER));
		add_identifier_e(&map->identifiers, identifier, curr->value);
		if (free(identifier), fte_flagged())
			return ;
	}
}
