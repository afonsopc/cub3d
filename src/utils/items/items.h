/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   items.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:50:57 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/06 19:54:48 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITEMS_H
# define ITEMS_H

# include <cub3d.h>

// Item
void	init_item(t_game *game, t_ftm_window *window, t_item *item,
			char identifier);
void	clear_food(void *data);
void	item_frame(t_item *item);
void	item_use(t_item *item, t_entity *user);

// Food
void	food_use(t_item *item, t_entity *user);
void	clear_item(void *data);
void	food_frame(t_item *item);
void	init_food(t_game *game, t_ftm_window *window, t_food *food,
			char identifier);

// Weapon
void	weapon_use(t_item *item, t_entity *user);
void	clear_weapon(void *data);
void	weapon_frame(t_item *item);
void	init_weapon(t_game *game, t_ftm_window *window, t_weapon *weapon,
			char identifier);

#endif
