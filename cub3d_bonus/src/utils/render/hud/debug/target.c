/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 21:41:28 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/18 20:51:24 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

static void	render_other_half_target(t_game *game, t_ftm_image *canvas,
	t_coords coords, t_entity *target_entity)
{
	char	*target_id;
	char	*target_type;
	char	*target_health;

	target_id = ft_strf("ID %c", target_entity->identifier);
	target_type = ft_strf("Type %d", target_entity->type);
	target_health = ft_strf("Health %d", target_entity->health);
	coords.x += render_debug_str(game, canvas, target_id, coords) + 5;
	coords.x += render_debug_str(game, canvas, target_health, coords) + 5;
	render_debug_str(game, canvas, target_type, coords);
	(free(target_id), free(target_type), free(target_health));
}

void	render_debug_target(t_game *game, t_ftm_image *canvas,
	t_coords coords, t_character *character)
{
	t_entity	*target_entity;
	char		*target_x;
	char		*target_y;
	char		*target_yaw;

	target_entity = character->target_entity;
	if (!target_entity)
		return ;
	target_x = ft_strf("X: %d", (int)target_entity->coords.x);
	target_y = ft_strf("Y: %d", (int)target_entity->coords.y);
	target_yaw = ft_strf("Yaw: %d",
			(int)target_entity->coords.yaw);
	coords.x += render_debug_str(game, canvas, "Target", coords) + 5;
	coords.x += render_debug_str(game, canvas, target_x, coords) + 5;
	coords.x += render_debug_str(game, canvas, target_y, coords) + 5;
	coords.x += render_debug_str(game, canvas, target_yaw, coords) + 5;
	(free(target_x), free(target_y), free(target_yaw));
	render_other_half_target(game, canvas, coords, target_entity);
}
