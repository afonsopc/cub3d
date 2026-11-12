/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:54:53 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 19:57:47 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATS_H
# define STATS_H

# include "../hud.h"

typedef struct s_hud_box
{
	int		x0;
	int		x1;
	t_size	stats_size;
}	t_hud_box;

void	render_score_text(t_game *game, t_ftm_image *canvas,
			t_character *character, t_size stats_size);
void	render_ammo_text(t_game *game, t_ftm_image *canvas,
			t_character *character, t_size stats_size);
void	render_health_text(t_game *game, t_ftm_image *canvas,
			t_character *character, t_size stats_size);
t_size	get_hand_item_size(t_ftm_image *image, double max_w,
			double max_h);
void	render_hand_item_icon(t_ftm_image *canvas, t_character *character,
			t_size stats_size);

#endif
