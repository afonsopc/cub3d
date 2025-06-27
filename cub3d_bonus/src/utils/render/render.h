/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 22:23:25 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 20:17:56 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <cub3d.h>

typedef struct s_raycasting_slice
{
	t_ftm_image	*canvas;
	t_player	*player;
	int			starting_index;
	int			ending_index;
}	t_raycasting_slice;

typedef struct s_camera_config
{
	double			fov;
	int				rays;
}	t_camera_config;

typedef struct s_player_canvas_data
{
	t_size		size;
	t_coords	coords;
}	t_player_canvas_data;

void	render_hud(t_game *game, t_ftm_image *canvas, t_character *character);
void	render_camera(t_game *game, t_ftm_image *canvas,
			t_character *character);

#endif