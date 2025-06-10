/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:45:35 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/10 18:05:57 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "walls.h"

static t_coords	get_coords(t_ray *ray)
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
	t_ray		ray;
	t_coords	coords;

	coords = drc.coords;
	coords.yaw = drc.yaw;
	ray = send_ray(drc.game, coords,
			drc.ignored_entity);
	if (!ray.hit_entity)
		return ;
	ray.distance += drc.previous_distance;
	if (ray.distance > drc.camera->ray_distances[drc.i])
		drc.camera->ray_distances[drc.i] = ray.distance;
	if (ray.hit_entity->transparent && entity_x_is_transparent(ray.hit_entity,
		ray.hit_direction, ray.hit_x))
	{
		drc.coords = get_coords(&ray);
		drc.ignored_entity = ray.hit_entity;
		drc.previous_distance = ray.distance;
		draw_ray(drc);
	}
	draw_ray_line(drc.canvas, drc.camera,
		ray, drc.i);
	if (drc.i == drc.camera->rays / 2)
	{
		drc.camera->character->target_entity = ray.hit_entity;
		drc.camera->character->target_entity_direction = ray.hit_direction;
		if (!ray.hit_entity->targetable)
			drc.camera->character->target_entity = NULL;
	}
}

static double	get_ray_yaw(t_draw_ray_config drc)
{
	double	half_rays;
	double	dist_to_proj;
	double	angle_ajustment;

	half_rays = drc.camera->rays / 2;
	dist_to_proj = half_rays / tan(ft_radians(drc.camera->fov) / 2);
	angle_ajustment = ft_degrees(atan(((double)drc.i - half_rays) / dist_to_proj));
	return (ft_normalize_angle(drc.camera->character->billboard.entity.coords.yaw + angle_ajustment));
}

static void	thread_render_rays(void *data)
{
	t_thread_render_rays_data	*trrd;
	t_draw_ray_config			drc;
	unsigned int				i;

	trrd = data;
	i = trrd->start;
	drc = (t_draw_ray_config){trrd->canvas, trrd->camera, trrd->game,
		trrd->camera->character->billboard.entity.coords, (t_entity *)trrd->camera->character, 0, 0, 0};
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
	static t_thread_render_rays_data	trrd[CAMERA_THREADS];
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
