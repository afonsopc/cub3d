/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:57 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 16:00:34 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DROP_H
# define DROP_H

# include "../entities.h"

void	set_sprite(t_drop	*drop);
bool	add_ammo_if_already_has_the_weapon(t_character *character,
			t_weapon *weapon);
void	do_the_thing(t_game *game, t_drop *drop);

#endif
