/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:20:20 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 21:21:17 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "loop.h"

static void	update_frame(t_game *game)
{
	t_time	now;

	now = ft_get_time();
	game->fps.delta_time = (now - game->fps.last_frame_time) / 1000.0f;
	game->fps.last_frame_time = now;
	++game->fps.frame_count;
	if (now - game->fps.fps_update_time > 250)
	{
		game->fps.fps = game->fps.frame_count * 4;
		game->fps.frame_count = 0;
		game->fps.fps_update_time = now;
		if (now - game->fps.beginning < 3000)
		{
			game->fps.min = game->fps.fps;
			return ;
		}
		if (game->fps.fps > game->fps.max)
			game->fps.max = game->fps.fps;
		if (game->fps.fps < game->fps.min)
			game->fps.min = game->fps.fps;
	}
}

static void	process_fps_limit(t_game *game)
{
	static t_time	previous_time;
	t_time			current_time;
	t_time			elapsed;
	double			target_frame_time;

	if (!previous_time)
	{
		previous_time = ft_get_time();
		return ;
	}
	target_frame_time = 1000.0 / game->fps.fps_limit;
	current_time = ft_get_time();
	elapsed = current_time - previous_time;
	if (elapsed < target_frame_time)
		ft_sleep(target_frame_time - elapsed);
	previous_time = ft_get_time();
}

static void	load_new_map(bool *playing_bg_music)
{
	char		*path;

	pthread_mutex_lock(&cub3d()->game_mutex);
	path = ft_strdup(cub3d()->new_map_path);
	cub3d()->new_map_path = NULL;
	if (!path)
		return (fte_set("load new map path"), fte_assert());
	ftm_put_image_to_window_pitc(cub3d()->window, cub3d()->loading_image,
		(t_ftm_pitc_config){.coords = {0, 0, 0}, .pixel_modifier = NULL,
		.resize = true, .size = cub3d()->window->size, .crop = false});
	ft_sleep(1000);
	(free_game(cub3d()->game), free_map(cub3d()->curr_map));
	cub3d()->game = NULL;
	cub3d()->curr_map = NULL;
	fta_destroy();
	cub3d()->curr_map = parse_map_e(path);
	(free(path), fte_assert(), fta_init_e());
	cub3d()->game = game_new_e(cub3d()->window, cub3d()->curr_map);
	fte_assert();
	*playing_bg_music = false;
	pthread_mutex_unlock(&cub3d()->game_mutex);
}

void	loop(void)
{
	static bool		playing_bg_music;

	if (cub3d()->new_map_path)
		load_new_map(&playing_bg_music);
	if (!cub3d()->game)
		return (ft_sleep(100));
	if (!playing_bg_music)
		fta_play(cub3d()->game->background_sound);
	playing_bg_music = true;
	pthread_mutex_lock(&cub3d()->game_mutex);
	update_frame(cub3d()->game);
	call_entity_frames(cub3d()->game, &cub3d()->game->fps);
	update_walls_matrix(cub3d()->game);
	update_billboards_vec(cub3d()->game);
	render_players_game(cub3d()->game, cub3d()->window);
	process_fps_limit(cub3d()->game);
	pthread_mutex_unlock(&cub3d()->game_mutex);
}
