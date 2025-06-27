/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:59:30 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 20:01:49 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hud.h"

void	render_effects(t_ftm_image *canvas, t_character *character)
{
	t_time	time_since_last_hit;
	t_time	time_since_last_auto_use;
	int		alpha;

	time_since_last_hit = ft_get_time() - character->last_hit;
	if (time_since_last_hit < DAMAGE_EFF_DUR)
	{
		alpha = 75 * (DAMAGE_EFF_DUR - time_since_last_hit) / DAMAGE_EFF_DUR;
		ftm_draw_rectangle(canvas, (t_coords){0, 0, 0}, canvas->size,
			(t_ftm_rectangle){(alpha << 24) | 0x00FF0000, 0, (t_size){0}});
	}
	time_since_last_auto_use = ft_get_time() - character->last_auto_use;
	if (time_since_last_auto_use < USE_EFF_DUR)
	{
		alpha = 75 * (USE_EFF_DUR - time_since_last_auto_use) / USE_EFF_DUR;
		ftm_draw_rectangle(canvas, (t_coords){0, 0, 0}, canvas->size,
			(t_ftm_rectangle){(alpha << 24) | 0x00FFFFFF, 0, (t_size){0}
		});
	}
	if (character->dead)
		ftm_draw_rectangle(canvas, (t_coords){0, 0, 0}, canvas->size,
			(t_ftm_rectangle){0xAAFF0000, 0, (t_size){0, 0}});
}
