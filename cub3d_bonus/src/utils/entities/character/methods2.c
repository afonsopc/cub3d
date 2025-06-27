/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 22:44:33 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:45:34 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "character.h"

void	free_inventory_items(t_character *character)
{
	int	i;

	i = -1;
	while (++i < INVENTORY_SIZE)
		free_item(character->inventory[i]);
}
