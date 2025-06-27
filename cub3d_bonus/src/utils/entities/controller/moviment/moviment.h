/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moviment.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:36:23 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:38:27 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVIMENT_H
# define MOVIMENT_H

# include "../controller.h"

void		looks(t_entity *entity, double delta_time);
t_entity	*position_overlaps(t_list *entities, t_entity *entity,
				t_coords coords);
void		move_player_x(t_list *entities, t_entity *entity, double angle,
				double velocity);
void		move_player_y(t_list *entities, t_entity *entity, double angle,
				double velocity);

#endif
