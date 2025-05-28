/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 22:23:25 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/28 17:56:56 by paude-so         ###   ########.fr       */
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

void	render_hud(t_game *game, t_ftm_image *canvas);
void	render_camera(t_game *game, t_ftm_image *canvas, t_camera *camera);

#endif