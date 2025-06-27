/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:45:35 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/26 02:33:57 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "walls.h"

static t_coords	get_coords(t_raycast *ray)
{
	t_coords	coords;

	coords = ray->hit_coords;
	if (ray->hit_direction == WEST || ray->hit_direction == EAST)
		coords.y += ray->hit_x;
	else if (ray->hit_direction == NORTH || ray->hit_direction == SOUTH)
		coords.x += ray->hit_x;
	if (ray->hit_direction == EAST)
		coords.x += 1.0;
	else if (ray->hit_direction == SOUTH)
		coords.y += 1.0;
	return (coords);
}

static void	draw_ray(t_draw_ray_config drc)
{
	t_raycast	ray;
	t_coords	c;

	c = drc.coords;
	c.yaw = drc.yaw;
	ray = ft_dda_raycast((t_dda_raycast_config){
			(void ***)drc.game->walls, drc.game->map->size, c,
			drc.ignored_entity});
	if (!ray.hit)
		return ;
	ray.distance += drc.previous_distance;
	if (drc.add_distance && ray.distance > drc.camera->ray_distances[drc.i])
		drc.camera->ray_distances[drc.i] = ray.distance;
	drc.add_distance = drc.add_distance
		&& !((t_entity *)ray.hit)->no_transparency_for_bill;
	if (((t_entity *)ray.hit)->ultra_mega_transparent
		|| (((t_entity *)ray.hit)->transparent
			&& entity_x_is_transparent(ray.hit, ray.hit_direction, ray.hit_x)))
	{
		drc.coords = get_coords(&ray);
		drc.ignored_entity = ray.hit;
		drc.previous_distance = ray.distance;
		draw_ray(drc);
	}
	draw_ray_line(drc.canvas, drc.camera, ray, drc.i);
}

static double	get_ray_yaw(t_draw_ray_config drc)
{
	double	half_rays;
	double	dist_to_proj;
	double	angle_ajustment;

	half_rays = drc.camera->rays / 2;
	dist_to_proj = half_rays / tan(ft_radians(drc.camera->fov) / 2);
	angle_ajustment = ft_degrees(atan(((double)drc.i - half_rays)
				/ dist_to_proj));
	return (ft_normalize_angle(
			drc.camera->character->billboard.entity.coords.yaw
			+ angle_ajustment));
}

static void	thread_render_rays(void *data)
{
	t_thread_render_rays_data	*trrd;
	t_draw_ray_config			drc;
	unsigned int				i;

	trrd = data;
	i = trrd->start;
	drc = (t_draw_ray_config){trrd->canvas, trrd->camera, trrd->game,
		trrd->camera->character->billboard.entity.coords,
		(t_entity *)trrd->camera->character, true, 0, 0, 0};
	while (i < trrd->end)
	{
		drc.i = i++;
		drc.yaw = get_ray_yaw(drc);
		drc.camera->ray_distances[drc.i] = -1;
		draw_ray(drc);
	}
}

void	render_walls(t_game *game, t_ftm_image *canvas, t_camera *camera)
{
	t_thread_render_rays_data			trrd[CAMERA_THREADS];
	unsigned int						i;
	unsigned int						index_scaler;

	i = -1;
	index_scaler = (camera->rays / CAMERA_THREADS);
	while (++i < CAMERA_THREADS)
	{
		trrd[i].canvas = canvas;
		trrd[i].camera = camera;
		trrd[i].game = game;
		trrd[i].start = index_scaler * i;
		trrd[i].end = index_scaler * (i + 1);
		game->camera_threads[i]->routine = thread_render_rays;
		game->camera_threads[i]->data = &trrd[i];
		ftt_thread_run(game->camera_threads[i]);
	}
	i = -1;
	while (++i < CAMERA_THREADS)
		ftt_thread_wait(game->camera_threads[i]);
}
