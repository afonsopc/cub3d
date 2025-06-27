/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 19:59:12 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/26 11:10:27 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hud.h"

void	render_hud(t_game *game, t_ftm_image *canvas, t_character *character)
{
	t_size	stats_size;

	if (!game->hud.enabled)
		return ;
	render_effects(canvas, character);
	stats_size = (t_size){0, 0};
	if (game->hud.stats_enabled)
		stats_size = render_stats(game, canvas, character);
	render_hand_item(canvas, character, stats_size);
	if (game->hud.minimap_enabled)
		render_minimap(game, canvas, character);
	if (game->hud.action_enabled)
		render_action(game, canvas, character);
	if (game->hud.debug_enabled)
		render_debug(game, canvas, character);
}
