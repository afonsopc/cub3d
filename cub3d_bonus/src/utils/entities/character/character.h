/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   character.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:57 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:45:39 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHARACTER_H
# define CHARACTER_H

# include "../entities.h"

void	set_using(t_game *game, t_character *character);
bool	walking(t_entity *entity);
bool	using(t_character *character);
void	call_item_frames(t_character *character);
void	drop_items(t_game *game, t_character *character);
void	free_inventory_items(t_character *character);

#endif
