/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   items.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:57 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/21 01:25:21 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITEMS_H
# define ITEMS_H

# include <cub3d.h>

// Item
void	init_item(t_game *game, t_ftm_window *window, t_item *item,
			char identifier);
void	clear_collectible(void *data);
void	item_use(t_item *item, t_drop *drop);
void	item_drop(t_game *game, t_ftm_window *window, t_item *item,
			t_character *dropper);
void	item_frame(t_item *item);

// Collectible
void	clear_item(void *data);
void	collectible_frame(t_item *item);
void	collectible_use(t_item *item, t_drop *drop);
void	collectible_drop(t_game *game, t_ftm_window *window, t_item *item,
			t_character *dropper);
void	init_collectible(t_game *game, t_ftm_window *window,
			t_collectible *collectible, char identifier);

// Weapon
void	clear_weapon(void *data);
void	weapon_frame(t_item *item);
void	weapon_use(t_item *item, t_drop *drop);
void	weapon_drop(t_game *game, t_ftm_window *window, t_item *item,
			t_character *dropper);
void	init_weapon(t_game *game, t_ftm_window *window, t_weapon *weapon,
			char identifier);

#endif
