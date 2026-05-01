/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:59:45 by afpachec          #+#    #+#             */
/*   Updated: 2025/08/13 12:54:13 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "walls.h"

void	draw_ray_line(t_ftm_image *canvas, t_camera *camera, t_raycast ray,
	int i)
{
	t_size					ray_size;
	t_ftm_image				*hit_entity_image;
	double					x_of_hit;
	t_ftm_blit_column_args	args;

	ray_size.width = canvas->size.width / camera->rays;
	hit_entity_image = get_sprite_image(get_entity_sprite(ray.hit,
				ray.hit_direction));
	if (!hit_entity_image)
		return ;
	ray_size.height = canvas->size.height / (fmax(ray.distance, 0.1)
			* ft_cos_degrees((ray.yaw
					- camera->character->billboard.entity.coords.yaw)));
	ray_size.height = fmin(ray_size.height, canvas->size.height * 3);
	x_of_hit = ray.hit_x;
	if (ray.hit_direction == NORTH || ray.hit_direction == EAST)
		x_of_hit = 1.0 - ray.hit_x;
	args.dst_x = i * ray_size.width;
	args.dst_y = (canvas->size.height - ray_size.height) / 2;
	args.dst_w = ray_size.width;
	args.dst_h = ray_size.height;
	args.src_col = (int)(x_of_hit * hit_entity_image->size.width);
	ftm_blit_column_fast(canvas, hit_entity_image, args);
}
