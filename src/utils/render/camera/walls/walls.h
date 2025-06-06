/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:40:06 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/24 00:46:07 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WALLS_H
# define WALLS_H

# include "../camera.h"

typedef struct s_dda_ray
{
	double		length;
	double		angle;
	t_coords	ray_dir;
	t_coords	delta_dist;
	t_coords	side_dist;
	t_coords	map_pos;
	t_coords	step;
	int			side;
	double		wall_x;
	t_entity	*hit_entity;
	t_direction	direction_of_hit_on_entity;
	t_entity	*ignored_entity;
	t_size		map_size;
}	t_dda_ray;

typedef struct s_ray
{
	double		distance;
	double		yaw;
	t_entity	*hit_entity;
	double		hit_x;
	t_direction	hit_direction;
	t_coords	hit_coords;
}	t_ray;

typedef struct s_draw_ray_config
{
	t_ftm_image		*canvas;
	t_camera		*camera;
	t_game			*game;
	t_coords		coords;
	t_entity		*ignored_entity;
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
	t_ray					*ray;
	t_ftm_image				*canvas;
	t_pixel_modifier_data	*pixel_modifier_data;
}	t_get_pitc_config_config;

t_ray	send_ray(t_game *game, t_coords coords, t_entity *ignored_entity);
void	draw_ray_line(t_ftm_image *canvas, t_camera *camera, t_ray ray, int i);

#endif
