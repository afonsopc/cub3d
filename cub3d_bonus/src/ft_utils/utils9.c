/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils9.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 14:18:24 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/20 10:46:40 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

t_list	*ft_list_new(void *data, void (*data_free)(void *data))
{
	t_list	*list;

	list = ft_calloc(1, sizeof(t_list));
	if (!list)
		return (NULL);
	list->data = data;
	list->data_free = data_free;
	list->next = NULL;
	return (list);
}

void	ft_list_add(t_list **list, void *data, void (*data_free)(void *data))
{
	t_list	*new_node;
	t_list	*curr;

	new_node = ft_list_new(data, data_free);
	if (!new_node)
		return ;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	curr = *list;
	while (curr->next)
		curr = curr->next;
	curr->next = new_node;
}

void	ft_list_destroy(t_list **list)
{
	if (!list)
		return ;
	ft_list_free(*list);
	*list = NULL;
}

size_t	ft_list_size(t_list *list)
{
	size_t	size;

	size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}

char	**ft_list_to_strv(t_list *list, char *(*to_str)(void *data))
{
	size_t	size;
	char	**strv;
	size_t	i;

	size = ft_list_size(list);
	strv = ft_calloc(size + 1, sizeof(char *));
	if (!strv)
		return (NULL);
	i = 0;
	while (list)
	{
		strv[i] = to_str(list->data);
		list = list->next;
		i++;
	}
	return (strv);
}
