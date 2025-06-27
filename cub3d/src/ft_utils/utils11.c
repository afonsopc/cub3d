/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils11.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:52:26 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/27 22:43:08 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

void	ft_hashmap_set(t_hashmap *hashmap, char *key, void *value,
	void (*el_free)(void *value))
{
	t_element	*existing_el;
	t_element	*new_el;

	if (!hashmap || !key)
		return ;
	existing_el = ft_hashmap_get(hashmap, key);
	if (existing_el)
		ft_hashmap_del(hashmap, key);
	new_el = ft_element_new(key, value, el_free);
	if (!new_el)
		return ;
	new_el->next = hashmap->table[0];
	hashmap->table[0] = new_el;
}

void	ft_hashmap_destroy(t_hashmap *hashmap)
{
	t_element	*curr_el;
	t_element	*next_el;

	if (!hashmap)
		return ;
	curr_el = hashmap->table[0];
	while (curr_el)
	{
		next_el = curr_el->next;
		ft_hashmap_del(hashmap, curr_el->key);
		curr_el = next_el;
	}
	free(hashmap->table);
	free(hashmap);
}

size_t	ft_hashmap_size(t_hashmap *hashmap)
{
	size_t		size;
	t_element	*curr_el;

	if (!hashmap)
		return (0);
	size = 0;
	curr_el = hashmap->table[0];
	while (curr_el)
	{
		size++;
		curr_el = curr_el->next;
	}
	return (size);
}

char	**ft_hashmap_to_strv(t_hashmap *hashmap)
{
	size_t		i;
	t_element	*curr_el;
	char		*tmp;
	char		**strv;

	if (!hashmap)
		return (NULL);
	strv = ft_calloc(ft_hashmap_size(hashmap) + 1, sizeof(char *));
	if (!strv)
		return (NULL);
	i = 0;
	curr_el = hashmap->table[0];
	while (curr_el)
	{
		tmp = ft_strjoin(curr_el->key, "=");
		strv[i] = ft_strjoin(tmp, curr_el->value);
		free(tmp);
		curr_el = curr_el->next;
		i++;
	}
	return (strv);
}

void	ft_fprint_hashmap(int fd, t_hashmap *hashmap)
{
	char	**as_strv;

	if (!hashmap)
		return ;
	as_strv = ft_hashmap_to_strv(hashmap);
	if (!as_strv)
		return ;
	ft_fprint_strv(fd, as_strv);
	ft_strvfree(as_strv);
}
