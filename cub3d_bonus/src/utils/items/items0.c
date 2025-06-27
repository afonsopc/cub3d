/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   items0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:15:02 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/20 10:23:18 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "items.h"

void	free_item(void *data)
{
	t_item	*item;

	item = data;
	if (!item)
		return ;
	if (item->clear)
		item->clear(data);
	free(item);
}
