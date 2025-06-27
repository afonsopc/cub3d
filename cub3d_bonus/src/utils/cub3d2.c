/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifiers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:13:02 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 21:13:09 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

static bool	identifier_is_char(t_hashmap *ids, char *type, char identifier)
{
	t_list	**id;

	id = ft_hashmap_get_value(ids, type);
	if (id)
		return (ft_list_any(*id, (void *)ft_str_equal_char_ptr, &identifier));
	return (false);
}

void	*hashmap_get_with_identifier(t_game *game, t_hashmap *hashmap,
	char identifier, char *rest)
{
	char	*key;
	void	*data;

	key = ft_strdup(rest);
	if (!rest)
		key = (free(key), ft_strndup(&identifier, 1));
	else if (identifier)
		key = (free(key), ft_strf("%c_%s", identifier, rest));
	data = ft_hashmap_get_value(hashmap, key);
	if (!data && identifier_is_char(game->map->identifiers, "PLAYER",
			identifier))
		data = ft_hashmap_get_value(hashmap, rest);
	return (free(key), data);
}

t_type_creator	get_type_creator(t_hashmap *ids, char identifier)
{
	if (identifier_is_char(ids, "ITEM", identifier))
		return ((void *)item_new);
	if (identifier_is_char(ids, "COLLECTIBLE", identifier))
		return ((void *)collectible_new);
	if (identifier_is_char(ids, "WEAPON", identifier))
		return ((void *)weapon_new);
	if (identifier_is_char(ids, "WALL", identifier))
		return ((void *)wall_new);
	if (identifier_is_char(ids, "PLAYER", identifier))
		return ((void *)player_new);
	if (identifier_is_char(ids, "DOOR", identifier))
		return ((void *)door_new_e);
	if (identifier_is_char(ids, "BILLBOARD", identifier))
		return ((void *)billboard_new);
	if (identifier_is_char(ids, "DROP", identifier))
		return ((void *)drop_new);
	if (identifier_is_char(ids, "CHARACTER", identifier))
		return ((void *)character_new);
	if (identifier_is_char(ids, "ELEVATOR", identifier))
		return ((void *)elevator_new);
	return (NULL);
}
