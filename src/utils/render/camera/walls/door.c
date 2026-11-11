/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/11 18:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/11 18:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "walls.h"

/*
** Doors are drawn like in the early-90s shooters: a thin panel on the
** cell's mid-line, half a block back from the wall faces, so the two side
** walls of the cell (the jambs, door-frame texture) show. Opening slides the
** panel along that mid-line into the wall at the low side of the cell (x-1
** for NORTH/SOUTH doors, y-1 for EAST/WEST doors), the same way from both
** sides. Secret push-walls (CLOSEABLE FALSE) keep the old flush look.
*/

bool	door_is_panel(t_entity *entity)
{
	return (entity && entity->type == ENTITY_DOOR
		&& ((t_door *)entity)->closeable);
}

double	door_open_amount(t_door *door)
{
	if (!door->opening_sprite || door->animation_frames <= 1)
		return (0.0);
	return (fmin(1.0, fmax(0.0, (double)door->opening_sprite->index
				/ (door->animation_frames - 1))));
}

/* t: distance from start along the ray to the mid-line; u: position on it */
static bool	midline_cross(t_door *door, t_coords start, t_coords cell,
	double tu[2])
{
	double	rdx;
	double	rdy;

	rdx = ft_cos_degrees(start.yaw);
	rdy = ft_sin_degrees(start.yaw);
	if (door->direction == NORTH || door->direction == SOUTH)
	{
		if (fabs(rdy) < FT_EPSILON)
			return (false);
		tu[0] = (cell.y + 0.5 - start.y) / rdy;
		tu[1] = start.x + tu[0] * rdx - cell.x;
	}
	else
	{
		if (fabs(rdx) < FT_EPSILON)
			return (false);
		tu[0] = (cell.x + 0.5 - start.x) / rdx;
		tu[1] = start.y + tu[0] * rdy - cell.y;
	}
	return (tu[0] >= 0 && tu[1] >= 0 && tu[1] < 1);
}

/*
** Returns true and fills *out (distance from start, texture x in hit_x) when
** the ray meets the closed part of the panel; false when it passes through
** the open gap (or misses the mid-line inside the cell).
*/
bool	door_panel_hit(t_raycast *ray, t_coords start, t_raycast *out)
{
	t_door	*door;
	double	tu[2];
	double	open;

	door = (t_door *)ray->hit;
	if (!midline_cross(door, start, ray->hit_coords, tu))
		return (false);
	open = door_open_amount(door);
	if (tu[1] > 1.0 - open)
		return (false);
	*out = *ray;
	out->distance = tu[0];
	out->hit_x = fmin(0.999, tu[1] + open);
	if (door->direction == NORTH || door->direction == SOUTH)
		out->hit_direction = ft_ternary_int(ft_sin_degrees(start.yaw) > 0,
				NORTH, SOUTH);
	else
		out->hit_direction = ft_ternary_int(ft_cos_degrees(start.yaw) > 0,
				WEST, EAST);
	return (true);
}

void	draw_door_line(t_ftm_image *canvas, t_camera *camera, t_raycast ray,
	int i)
{
	t_size					ray_size;
	t_ftm_image				*image;
	t_ftm_blit_column_args	args;

	ray_size.width = canvas->size.width / camera->rays;
	image = get_sprite_image(((t_door *)ray.hit)->door_sprite);
	if (!image)
		return ;
	ray_size.height = canvas->size.height / (fmax(ray.distance, 0.1)
			* ft_cos_degrees((ray.yaw
					- camera->character->billboard.entity.coords.yaw)));
	ray_size.height = fmin(ray_size.height, canvas->size.height * 3);
	args.dst_x = i * ray_size.width;
	args.dst_y = (canvas->size.height - ray_size.height) / 2;
	args.dst_w = ray_size.width;
	args.dst_h = ray_size.height;
	args.src_col = (int)(ray.hit_x * image->size.width);
	ftm_blit_column_fast(canvas, image, args);
}
