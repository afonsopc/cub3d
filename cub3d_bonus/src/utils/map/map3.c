/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:52:20 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 21:03:38 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

static bool	identifier_already_defined(t_hashmap *identifiers, char *identifier)
{
	t_element	*el;
	t_list		**curr;

	el = identifiers->table;
	while (el)
	{
		curr = el->value;
		el = el->next;
		if (curr && ft_list_any(*curr, (void *)ft_strequal, identifier))
			return (true);
	}
	return (false);
}

static void	id_list_free(t_list **id_list)
{
	ft_list_destroy(id_list);
	free(id_list);
}

static void	add_identifier(t_hashmap *identifiers, char *identifier, char *type)
{
	t_list	**ids;

	ids = (t_list **)ft_hashmap_get_value(identifiers, type);
	if (!ids)
	{
		ft_hashmap_set(identifiers, type, ft_calloc(1, sizeof(t_list *)),
			(void *)id_list_free);
		ids = (t_list **)ft_hashmap_get_value(identifiers, type);
		if (!ids)
			return ;
	}
	ft_list_add(ids, ft_strdup(identifier), free);
}

static void	add_default_identifiers(t_hashmap *identifiers)
{
	char	*id_str;
	int		i;

	i = -1;
	while (DEFAULT_AIR_TYPES[++i])
	{
		id_str = ft_strndup(&DEFAULT_AIR_TYPES[i], 1);
		(add_identifier(identifiers, id_str, "AIR"), free(id_str));
	}
	i = -1;
	while (DEFAULT_WALL_TYPES[++i])
	{
		id_str = ft_strndup(&DEFAULT_WALL_TYPES[i], 1);
		(add_identifier(identifiers, id_str, "WALL"), free(id_str));
	}
	i = -1;
	while (DEFAULT_PLAYER_TYPES[++i])
	{
		id_str = ft_strndup(&DEFAULT_PLAYER_TYPES[i], 1);
		(add_identifier(identifiers, id_str, "PLAYER"), free(id_str));
	}
}

void	parse_identifiers_e(t_map *map)
{
	char		*identifier;
	t_element	*el;
	t_element	*curr;

	fte_set(NULL);
	map->identifiers = ft_hashmap_new();
	if (!map->identifiers)
		return (fte_set("Identifier Hashmap Error (its so over)"));
	add_default_identifiers(map->identifiers);
	el = map->types->table;
	while (el)
	{
		curr = el;
		el = el->next;
		if (!ft_str_endswith(curr->key, "_TYPE"))
			continue ;
		identifier = ft_strndup(curr->key, ft_strlen(curr->key) - 5);
		if (!identifier || !*identifier)
			return (free(identifier), fte_set("empty type identifier"));
		if (identifier_already_defined(map->identifiers, identifier))
			return (fte_set("dup type id %s", identifier), free(identifier));
		add_identifier(map->identifiers, identifier, curr->value);
		if (free(identifier), fte_flagged())
			return ;
	}
}
