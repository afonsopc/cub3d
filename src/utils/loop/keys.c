/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:21:37 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/03 11:17:24 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loop.h"

static void	activate_another_player(t_game *game)
{
	int	i;

	if (!game)
		return ;
	i = -1;
	while (++i < PLAYER_MAX)
	{
		if (!game->players[i])
			return ;
		if (game->players[i]->character.billboard.entity.active)
			continue ;
		game->players[i]->character.billboard.entity.active = true;
		return ;
	}
}

static void	deactivate_another_player(t_game *game)
{
	int	i;

	if (!game)
		return ;
	i = PLAYER_MAX;
	while (--i)
	{
		if (!game->players[i]
			|| !game->players[i]->character.billboard.entity.active)
			continue ;
		game->players[i]->character.billboard.entity.active = false;
		return ;
	}
}

static void	hud_keys(t_game *game, t_ftm_key_hook_values khv)
{
	if (!game)
		return ;
	if (khv.key == FTM_KEY_F3 && khv.down)
		game->hud.debug_enabled = !game->hud.debug_enabled;
	if (khv.key == FTM_KEY_F2 && khv.down)
		game->hud.stats_enabled = !game->hud.stats_enabled;
	if (khv.key == FTM_KEY_F4 && khv.down)
		game->hud.minimap_enabled = !game->hud.minimap_enabled;
	if (khv.key == FTM_KEY_F5 && khv.down)
		game->hud.action_enabled = !game->hud.action_enabled;
	if (khv.key == FTM_KEY_F6 && khv.down)
		game->hud.enabled = !game->hud.enabled;
	if (khv.key == FTM_KEY_0 && khv.down)
		game->hud.minimap.zoom_level *= 1.2;
	if (khv.key == FTM_KEY_8 && khv.down)
		game->hud.minimap.zoom_level /= 1.2;
	if (khv.key == FTM_KEY_9 && khv.down)
		game->hud.minimap.zoom_level = 5.0;
}

static void	game_keys(t_game *game, t_ftm_key_hook_values khv)
{
	(void)game;
	if (khv.key == FTM_KEY_M && khv.down && cub3d()->window)
		cub3d()->window->using_mouse = !cub3d()->window->using_mouse;
}

void	key_hook(t_ftm_key_hook_values khv)
{
	t_game	*game;

	game = cub3d()->game;
	pthread_mutex_lock(&cub3d()->game_mutex);
	if (game != cub3d()->game)
		return (pthread_mutex_unlock(&cub3d()->game_mutex), (void)0);
	call_entity_keys(game, khv);
	if (khv.key == FTM_KEY_ESC)
	{
		cub3d()->new_map_path = DEFAULT_MAP_PATH;
		pthread_mutex_unlock(&cub3d()->game_mutex);
		return ; 
	}
	if (game && khv.key == FTM_KEY_K && khv.down && game->players[0])
		((t_entity *)game->players[0])->controller.keyboard_only
			= !((t_entity *)game->players[0])->controller.keyboard_only;
	if (khv.key == FTM_KEY_P && khv.down)
		activate_another_player(game);
	if (khv.key == FTM_KEY_O && khv.down)
		deactivate_another_player(game);
	if (khv.key == FTM_KEY_F7 && khv.down)
		ftm_window_reload_controllers(cub3d()->window);
	if (khv.key == FTM_KEY_F1 && khv.down)
		ftm_window_toggle_fullscreen(cub3d()->window, (t_size){W_WIDTH,
			W_HEIGHT});
	hud_keys(game, khv);
	game_keys(game, khv);
	pthread_mutex_unlock(&cub3d()->game_mutex);
}
