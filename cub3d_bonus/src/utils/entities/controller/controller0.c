/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:31:25 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/21 02:37:01 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"

void	init_controller(t_game *game, t_entity *entity, const char *type)
{
	if (ft_strequal(type, "PLAYER"))
		init_player_controller(entity);
	else if (ft_strequal(type, "AI_DUMB"))
		init_ai_dumb_controller(entity);
	else if (ft_strequal(type, "ENEMY"))
		init_enemy_controller(game, entity);
}
