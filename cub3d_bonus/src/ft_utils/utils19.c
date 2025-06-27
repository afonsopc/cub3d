/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils19.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 16:51:19 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:11:08 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

static void	init_ray(t_dda_raycast_data *ray, t_dda_raycast_config ddarc)
{
	ray->objs = ddarc.objs;
	ray->objs_size = ddarc.objs_size;
	ray->sp = ddarc.start_pos;
	ray->ignored_obj = ddarc.ignored_obj;
	ray->rdir.x = ft_cos_degrees(ddarc.start_pos.yaw);
	ray->rdir.y = ft_sin_degrees(ddarc.start_pos.yaw);
	ray->pos.x = (int)ddarc.start_pos.x;
	ray->pos.y = (int)ddarc.start_pos.y;
	ray->deltadist.x = ft_ternary_double(fabs(ray->rdir.x) < FT_EPSILON,
			FT_MAX_RAY_LENGTH, fabs(1.0 / ray->rdir.x));
	ray->deltadist.y = ft_ternary_double(fabs(ray->rdir.y) < FT_EPSILON,
			FT_MAX_RAY_LENGTH, fabs(1.0 / ray->rdir.y));
	ray->step.x = ft_ternary_double(ray->rdir.x < 0, -1, 1);
	ray->step.y = ft_ternary_double(ray->rdir.y < 0, -1, 1);
	ray->side_dist.x = ft_ternary_double(ray->rdir.x < 0, (ray->sp.x
				- ray->pos.x) * ray->deltadist.x, (ray->pos.x + 1.0 - ray->sp.x)
			* ray->deltadist.x);
	ray->side_dist.y = ft_ternary_double(ray->rdir.y < 0, (ray->sp.y
				- ray->pos.y) * ray->deltadist.y, (ray->pos.y + 1.0 - ray->sp.y)
			* ray->deltadist.y);
}

static void	*perform_dda_algorithm(t_dda_raycast_data *ray)
{
	void	*hit;

	while (true)
	{
		ray->side = ray->side_dist.x >= ray->side_dist.y;
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->deltadist.x;
			ray->pos.x += ray->step.x;
		}
		else
		{
			ray->side_dist.y += ray->deltadist.y;
			ray->pos.y += ray->step.y;
		}
		if (ray->side_dist.x >= FT_MAX_RAY_LENGTH
			&& ray->side_dist.y >= FT_MAX_RAY_LENGTH)
			return (NULL);
		if ((int)ray->pos.x < 0 || (int)ray->pos.x >= ray->objs_size.width
			|| (int)ray->pos.y < 0 || (int)ray->pos.y >= ray->objs_size.height)
			continue ;
		hit = ray->objs[(int)ray->pos.y][(int)ray->pos.x];
		if (hit && hit != ray->ignored_obj)
			return (hit);
	}
}

static void	set_aftermath(t_dda_raycast_data *r)
{
	if (!r->side)
	{
		r->length = ft_ternary_double(
				fabs(r->rdir.x) < FT_EPSILON, FT_MAX_RAY_LENGTH,
				((r->pos.x - r->sp.x + (1 - r->step.x) / 2.0) / r->rdir.x));
		r->hitdir = ft_ternary_int(r->step.x > 0, WEST, EAST);
		r->hit_x = ft_fraction(r->sp.y + r->length * r->rdir.y);
		return ;
	}
	r->length = ft_ternary_double(
			fabs(r->rdir.y) < FT_EPSILON, FT_MAX_RAY_LENGTH,
			((r->pos.y - r->sp.y + (1 - r->step.y) / 2.0) / r->rdir.y));
	r->hitdir = ft_ternary_int(r->step.y > 0, NORTH, SOUTH);
	r->hit_x = ft_fraction(r->sp.x + r->length * r->rdir.x);
}

t_raycast	ft_dda_raycast(t_dda_raycast_config ddarc)
{
	t_dda_raycast_data	ray;

	init_ray(&ray, ddarc);
	ray.hit = perform_dda_algorithm(&ray);
	if (!ray.hit)
		return ((t_raycast){FT_MAX_RAY_LENGTH, ddarc.start_pos.yaw, NULL, 0, 0,
			(t_coords){0, 0, 0}});
	set_aftermath(&ray);
	return ((t_raycast){
		ray.length,
		ddarc.start_pos.yaw,
		ray.hit,
		ray.hit_x,
		ray.hitdir,
		ray.pos
	});
}
