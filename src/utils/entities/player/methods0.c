/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/11 12:24:05 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"

void	player_frame(t_entity *entity, double delta_time)
{
	character_frame(entity, delta_time);
}

void	clear_player(void *player)
{
	clear_character(player);
}

void	player_action(t_entity *entity, t_character *actioner)
{
	character_action(entity, actioner);
}

void	player_shot(t_entity *shooted, t_character *shooter)
{
	character_shot(shooted, shooter);
}
