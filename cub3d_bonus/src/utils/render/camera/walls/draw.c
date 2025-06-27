/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:59:45 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 20:17:35 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "walls.h"

static t_ftm_pitc_config	get_pitc_config(t_get_pitc_config_config gpcc)
{
	double	x_of_hit;

	x_of_hit = gpcc.ray->hit_x;
	if (gpcc.ray->hit_direction == NORTH || gpcc.ray->hit_direction == EAST)
		x_of_hit = 1.0 - gpcc.ray->hit_x;
	return ((t_ftm_pitc_config){
		(t_coords){gpcc.i * gpcc.ray_size->width,
		(gpcc.canvas->size.height - gpcc.ray_size->height) / 2, 0},
		true,
		(t_coords){(int)(x_of_hit * gpcc.image->size.width), 0, 0},
		(t_coords){(int)(x_of_hit * gpcc.image->size.width) + 1,
		gpcc.image->size.height, 0},
		true,
		*gpcc.ray_size,
		NULL,
		NULL
	});
}

void	draw_ray_line(t_ftm_image *canvas, t_camera *camera, t_raycast ray,
	int i)
{
	t_size					ray_size;
	t_ftm_image				*hit_entity_image;
	t_pixel_modifier_data	pixel_modifier_data;

	ray_size.width = canvas->size.width / camera->rays;
	hit_entity_image = get_sprite_image(get_entity_sprite(ray.hit,
				ray.hit_direction));
	if (!hit_entity_image)
		return ;
	ray_size.height = canvas->size.height / (fmax(ray.distance, 0.1)
			* ft_cos_degrees((ray.yaw
					- camera->character->billboard.entity.coords.yaw)));
	ray_size.height = fmin(ray_size.height, canvas->size.height * 3);
	pixel_modifier_data = (t_pixel_modifier_data){canvas, &ray_size};
	ftm_put_image_to_canvas(canvas, hit_entity_image,
		get_pitc_config((t_get_pitc_config_config){i, &ray_size,
			hit_entity_image, &ray, canvas, &pixel_modifier_data}));
}
