/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 22:14:35 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 19:59:08 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stats.h"

static t_ftm_image	*get_stats_image(t_game *game, t_character *character)
{
	int	index;

	if (character->dead)
		return (get_sprite_image(game->hud.stats.dead_sprite));
	if (character->cheating)
		return (get_sprite_image(game->hud.stats.cheat_sprite));
	index = (int)(game->hud.stats.states
			* ((float)character->billboard.entity.health
				/ character->billboard.entity.max_health));
	index = game->hud.stats.states - index;
	if (index >= game->hud.stats.states)
		index = game->hud.stats.states - 1;
	if (index < 0)
		index = 0;
	return (get_sprite_image(ft_list_index(game->hud.stats.states_list,
				index)->data));
}

t_size	render_stats(t_game *game, t_ftm_image *canvas, t_character *character)
{
	t_size		stats_size;
	t_ftm_image	*image;

	if (!game->hud.stats_enabled || !game->hud.stats.states)
		return ((t_size){0, 0});
	stats_size = (t_size){canvas->size.width, canvas->size.width / 8};
	image = get_stats_image(game, character);
	ftm_put_image_to_canvas(canvas, image,
		(t_ftm_pitc_config){
		(t_coords){0, canvas->size.height - stats_size.height, 0},
		false, (t_coords){0, 0, 0}, (t_coords){0, 0, 0},
		true, stats_size, NULL, NULL});
	render_health_text(game, canvas, character, stats_size);
	render_ammo_text(game, canvas, character, stats_size);
	render_score_text(game, canvas, character, stats_size);
	render_hand_item_icon(canvas, character, stats_size);
	return (stats_size);
}
