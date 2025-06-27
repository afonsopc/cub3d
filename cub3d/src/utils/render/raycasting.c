/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 19:05:41 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/26 12:37:35 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	setup_wall_dimensions(t_render_data *data)
{
	double	angle_diff;

	angle_diff = data->player->rays[data->ray_idx].angle;
	angle_diff -= data->player->base.coords.yaw;
	angle_diff = ft_normalize_angle(angle_diff) * (PI / 180.0);
	data->wall_height = W_HEIGHT
		/ (fmax(data->player->rays[data->ray_idx].length, 0.1)
			* cos(angle_diff));
	data->draw_start = (W_HEIGHT - data->wall_height) / 2;
	if (data->draw_start < 0)
		data->draw_start = 0;
	data->draw_end = (W_HEIGHT + data->wall_height) / 2;
	if (data->draw_end >= W_HEIGHT)
		data->draw_end = W_HEIGHT - 1;
}

static t_ftm_image	*get_wall_texture(t_render_data *data)
{
	t_ftm_image	*texture;
	t_ray		*rays;
	int			ray_idx;

	rays = data->player->rays;
	ray_idx = data->ray_idx;
	texture = get_sprite_image(get_entity_sprite(rays[ray_idx].hit_entity,
				rays[ray_idx].hit_direction));
	if (!texture)
		return (NULL);
	data->tex_x = rays[ray_idx].x_of_hit_in_entity
		* texture->size.width;
	if (data->tex_x < 0)
		data->tex_x = 0;
	if (data->tex_x >= texture->size.width)
		data->tex_x = texture->size.width - 1;
	data->tex_y_step = texture->size.height / (double)data->wall_height;
	return (texture);
}

static void	draw_vertical_line(t_render_data *data, int x_offset)
{
	int				y;
	double			tex_pos;
	int				tex_y;
	unsigned int	*texture_data;
	unsigned int	*canvas_data;

	texture_data = (unsigned int *)data->texture->data;
	canvas_data = (unsigned int *)data->canvas->data;
	tex_pos = (data->draw_start - (W_HEIGHT - data->wall_height) / 2)
		* data->tex_y_step;
	y = data->draw_start;
	while (y < data->draw_end)
	{
		tex_y = (int)tex_pos % data->texture->size.height;
		if (tex_y < 0)
			tex_y += data->texture->size.height;
		tex_pos += data->tex_y_step;
		canvas_data[y * W_WIDTH + (data->screen_x
				+ x_offset)] = texture_data[tex_y * data->texture->size.width
			+ (int)data->tex_x];
		y++;
	}
}

static void	draw_vertical_strip(t_render_data *data)
{
	int	x;

	x = 0;
	while (x < data->strip_width)
	{
		draw_vertical_line(data, x);
		x++;
	}
}

void	render_raycasting_mega(t_game *game, t_ftm_image *canvas)
{
	t_render_data	data;
	int				i;

	data.player = game->player;
	data.canvas = canvas;
	data.width_scale = (double)W_WIDTH / PLAYER_RAYS;
	i = 0;
	while (i < PLAYER_RAYS)
	{
		if (data.player->rays[i].hit_entity)
		{
			data.ray_idx = i;
			data.screen_x = i * data.width_scale;
			data.strip_width = ceil((i + 1) * data.width_scale) - data.screen_x;
			setup_wall_dimensions(&data);
			data.texture = get_wall_texture(&data);
			if (data.texture)
				draw_vertical_strip(&data);
		}
		i++;
	}
}
