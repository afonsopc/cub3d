/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils21.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 00:27:36 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:11:34 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

bool	ft_list_all(t_list *list, bool (*condition)(void *data, void *params),
	void *params)
{
	t_list	*current;

	if (!list || !condition)
		return (false);
	current = list;
	while (current)
	{
		if (!condition(current->data, params))
			return (false);
		current = current->next;
	}
	return (true);
}

size_t	ft_list_count(t_list *list, bool (*condition)(void *data, void *params),
	void *params)
{
	size_t	count;

	if (!list || !condition)
		return (0);
	count = 0;
	while (list)
	{
		if (condition(list->data, params))
			count++;
		list = list->next;
	}
	return (count);
}
