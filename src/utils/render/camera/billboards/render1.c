/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:47:16 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/10 18:13:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "billboards.h"

static bool	is_behind_wall(int i, unsigned int ray_index, t_render_billboard_slices_config rbsc)
{
	double			offset;
	double			world_x;
	double			world_y;
	double			dist;

	if (ray_index >= rbsc.camera->rays)
		return (false);
	offset = (i - rbsc.new_size.width / 2.0) / rbsc.new_size.width;
	world_x = rbsc.bill_coords.x + offset
		* cos(ft_radians(rbsc.bill_coords.yaw + 90));
	world_y = rbsc.bill_coords.y + offset
		* sin(ft_radians(rbsc.bill_coords.yaw + 90));
	dist = ft_distance(rbsc.camera->character->billboard.entity.coords,
			(t_coords){world_x, world_y, 0});
	return (dist >= rbsc.camera->ray_distances[ray_index]);
}

static t_ftm_pitc_config	get_pitc_config(int i,
	t_render_billboard_slices_config rbsc)
{
	double	src_x_center;
	int		src_x_start;
	int		src_x_end;

	src_x_center = ((double)i / rbsc.new_size.width) * rbsc.image->size.width;
	src_x_start = (int)(src_x_center - 0.5);
	src_x_end = (int)(src_x_center + 0.5);
	if (src_x_start < 0)
		src_x_start = 0;
	if (src_x_end > rbsc.image->size.width)
		src_x_end = rbsc.image->size.width;
	return ((t_ftm_pitc_config){
		(t_coords){rbsc.screen_x - (rbsc.new_size.width / 2) + i,
		(rbsc.canvas->size.height / 2) - (rbsc.new_size.height / 2), 0},
		true, (t_coords){src_x_start, 0, 0},
		(t_coords){src_x_end, rbsc.image->size.height, 0},
		true, (t_size){1, rbsc.new_size.height}, NULL, NULL
	});
}

void	render_billboard_slices(t_render_billboard_slices_config rbsc)
{
	unsigned int		ray_index;
	t_ftm_pitc_config	pitc_config;
	int					i;

	i = -1;
	while (++i < rbsc.new_size.width)
	{
		ray_index = (int)((rbsc.screen_x + i - (rbsc.new_size.width / 2))
				/ rbsc.canvas->size.width * rbsc.camera->rays);
		if (is_behind_wall(i, ray_index, rbsc))
			continue ;
		pitc_config = get_pitc_config(i, rbsc);
		if (ray_index == rbsc.camera->rays / 2 && rbsc.bill->entity.targetable)
			rbsc.camera->character->target_entity = (t_entity *)rbsc.bill;
		ftm_put_image_to_canvas(rbsc.canvas, rbsc.image, pitc_config);
	}
}
