/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:49:41 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 19:51:42 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H

# include "../hud.h"

void	set_entity_color(t_minimap_data mp_data, t_entity *entity,
			unsigned int *color);
bool	in_boundaries(t_minimap_data mp_data, t_coords entity_pos);
void	fix_clipping(t_minimap_data mp_data, t_coords *pos, t_size *size);
void	render_entity(t_minimap_data mp_data, t_entity *entity,
			t_coords coords, t_size entity_size);
void	calculate_minimap_scale(t_minimap_data mp_data, t_dsize *scale,
			t_size *entity_size, t_coords *entity_offset);

#endif
