/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:21:37 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 16:44:13 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loop.h"

static void	hud_debug_keys(t_hud_debug *hud_debug, int key, bool down)
{
	t_size	map_size;

	if (key == XK_F3 && down)
		hud_debug->enabled = !hud_debug->enabled;
	if (!hud_debug->enabled)
		return ;
	if (key == XK_c && down)
	{
		if (cub3d()->game.camera.rays % 2)
			cub3d()->game.camera.rays += 1;
		else
			cub3d()->game.camera.rays /= 2;
	}
	if (key == XK_v && down)
		cub3d()->game.camera.rays = cub3d()->window.canvas->size.width;
	if (key == XK_b && down)
		cub3d()->game.player->character.billboard.entity.hard = !cub3d()->game.player->character.billboard.entity.hard;
	if (key == XK_n && down)
	{
		map_size = cub3d()->game.map->size;
		cub3d()->game.player->character.billboard.entity.coords = (t_coords){map_size.width / 2,
		map_size.height / 2, 0};
	}
	if (key == XK_m && down)
		cub3d()->window.using_mouse = !cub3d()->window.using_mouse;
	if (key == XK_k && down)
		cub3d()->game.fps.fps_limit /= 1.5;
	if (key == XK_l && down)
		cub3d()->game.fps.fps_limit = FPS_LIMIT;
	if (key == XK_o && down)
		cub3d()->game.camera.fov /= 1.2;
	if (key == XK_p && down)
		cub3d()->game.camera.fov *= 1.2;
	if (key == XK_y && down)
		cub3d()->game.player->character.billboard.entity.health /= 1.2;
	if (key == XK_u && down)
		cub3d()->game.player->character.billboard.entity.health = cub3d()->game.player->character.billboard.entity.max_health;
}

static void	hud_minimap_keys(int key, bool down)
{
	if (key == XK_Tab)
		cub3d()->game.hud.minimap.full = down;
	if (key == XK_equal && down && cub3d()->game.hud.minimap.zoom_level < 5.0)
		cub3d()->game.hud.minimap.zoom_level += 0.5;
	if (key == XK_minus && down && cub3d()->game.hud.minimap.zoom_level > 1.0)
		cub3d()->game.hud.minimap.zoom_level -= 0.5;
	if (key == XK_0 && down)
		cub3d()->game.hud.minimap.zoom_level = 5.0;
}

static void	hud_keys(t_game *game, t_ftm_window *window, int key, bool down)
{
    if (key == XK_h && down)
        game->hud.enabled = !game->hud.enabled;
    if (key == XK_F1 && down)
		game->camera.rays = ftm_window_toggle_fullscreen(window, (t_size){W_WIDTH, W_HEIGHT}).width;
	if (!game->hud.enabled)
		return ;
	hud_debug_keys(&game->hud.debug, key, down);
	hud_minimap_keys(key, down);
}

void	key_hook(int key, t_coords coords, bool down)
{
	(void)coords;
	call_entity_keys(cub3d()->game.entities, key, down);
	hud_keys(&cub3d()->game, &cub3d()->window, key, down);
	if (key == XK_Escape)
		cub3d_exit(0);
}
