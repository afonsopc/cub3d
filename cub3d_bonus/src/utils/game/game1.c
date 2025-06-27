/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:16:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 19:01:56 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	init_enviroment_e(t_game *game)
{
	char	*ceiling;
	char	*flooring;

	fte_set(NULL);
	ceiling = ft_hashmap_get_value(game->map->types, "C");
	flooring = ft_hashmap_get_value(game->map->types, "F");
	if (!ceiling || !flooring)
		return (fte_set("ceiling or flooring missing"));
	game->environment.ceiling_color = ft_rgb_to_unsigned(ceiling, ",");
	game->environment.floor_color = ft_rgb_to_unsigned(flooring, ",");
}

void	init_minimap(t_ftm_window *window, t_game *game)
{
	t_hud_minimap	*minimap;

	minimap = &game->hud.minimap;
	minimap->width_multiplier = MINIMAP_WIDTH_MULTIPLIER;
	minimap->height_multiplier = MINIMAP_HEIGHT_MULTIPLIER;
	minimap->coords = (t_coords){window->size.width
		- (window->size.width * minimap->width_multiplier) - 10, 10, 0};
	minimap->size = (t_size){window->size.width
		* game->hud.minimap.width_multiplier, window->size.height
		* game->hud.minimap.height_multiplier};
	minimap->background_color = 0xCC555555;
	minimap->border_color = 0xFFAAAAAA;
	minimap->wall_color = 0xFFAAAAAA;
	minimap->player_color = 0xFFFF0000;
	minimap->player_ray_color = 0xFF880088;
	minimap->player_middle_ray_color = 0xFFFF0000;
	minimap->entity_color = 0xFF888888;
	minimap->door_color = 0xFF8888DD;
	minimap->elevator_color = 0xFF88DDDD;
	minimap->zoom_level = 5.0;
}

void	init_window(t_ftm_window *window, t_game *game)
{
	window->using_mouse = ft_strequal(ft_hashmap_get_value(game->map->types,
				"MOUSE"), "TRUE");
}

void	init_font_e(t_ftm_window *window, t_game *game)
{
	t_ftm_font	*font;

	if (ft_hashmap_get_value(game->fonts, "DEFAULT"))
		return ;
	font = ftm_font_new(window, DEFAULT_FONT_PATH);
	if (!font)
		fte_set("default font not found :(");
	ft_hashmap_set(game->fonts, "DEFAULT", font, ftm_free_font);
}
