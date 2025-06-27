/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:40:06 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 00:35:34 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WALLS_H
# define WALLS_H

# include "../camera.h"

typedef struct s_draw_ray_config
{
	t_ftm_image		*canvas;
	t_camera		*camera;
	t_game			*game;
	t_coords		coords;
	t_entity		*ignored_entity;
	bool			add_distance;
	double			previous_distance;
	double			yaw;
	unsigned int	i;
}	t_draw_ray_config;

typedef struct s_thread_render_rays_data
{
	t_ftm_image		*canvas;
	t_camera		*camera;
	t_game			*game;
	unsigned int	start;
	unsigned int	end;
}	t_thread_render_rays_data;

typedef struct s_pixel_modifier_data
{
	t_ftm_image	*canvas;
	t_size		*ray_size;
}	t_pixel_modifier_data;

typedef struct s_get_pitc_config_config
{
	int						i;
	t_size					*ray_size;
	t_ftm_image				*image;
	t_raycast				*ray;
	t_ftm_image				*canvas;
	t_pixel_modifier_data	*pixel_modifier_data;
}	t_get_pitc_config_config;

void	draw_ray_line(t_ftm_image *canvas, t_camera *camera, t_raycast ray,
			int i);

#endif
