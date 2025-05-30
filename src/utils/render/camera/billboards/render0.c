/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:47:16 by paude-so          #+#    #+#             */
/*   Updated: 2025/05/30 21:11:14 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "billboards.h"

static double	get_relative_angle(t_coords camera_coords,
	t_coords bill_coords)
{
	double	relative_angle;

	relative_angle = ft_angle_distance(camera_coords, bill_coords);
	if (relative_angle > 180)
		relative_angle -= 360;
	return (relative_angle);
}

static t_size	get_size(t_get_size_config gsc)
{
	double		distance;
	double		fov_factor;
	double		fix_fisheye;

	distance = ft_distance(gsc.camera->entity->coords, gsc.bill_coords);
	fix_fisheye = distance * ft_cos_degrees(gsc.relative_angle);
	fov_factor = 73.5 / gsc.camera->fov;
	return ((t_size){(gsc.bill_image_size.width / distance)
		* (gsc.canvas_size.height / 125) * fov_factor,
		(gsc.bill_image_size.height / fix_fisheye)
		* (gsc.canvas_size.height / 125)});
}

static double	get_screen_x(t_ftm_image *canvas, t_camera *camera,
	double relative_angle)
{
	double	screen_x;

	screen_x = canvas->size.width / 2 + (relative_angle / (camera->fov / 2))
		* (canvas->size.width / 2);
	if (screen_x < canvas->size.width / 2)
		screen_x += 0.05 * ((canvas->size.width / 2) - screen_x);
	else
		screen_x -= 0.05 * (screen_x - (canvas->size.width / 2));
	return (screen_x);
}

static int	get_image_width(t_sprite *sprite)
{
	if (!sprite)
		return (0);
	return (get_sprite_image(sprite)->size.width);
}

static void	render_billboard(t_billboard *bill, t_ftm_image *canvas,
	t_camera *camera)
{
	t_ftm_image	*image;
	t_size		new_size;
	double		screen_x;
	double		relative_angle;
	t_coords	centered_bill_coords;

	centered_bill_coords = (t_coords){bill->entity.coords.x + 0.5,
		bill->entity.coords.y + 0.5, bill->entity.coords.yaw};
	relative_angle = get_relative_angle(camera->entity->coords,
			centered_bill_coords);
	image = get_sprite_image(bill->sprites[(int)ft_angle_distance(camera->entity->coords, centered_bill_coords)]);
	if (!image)
		return ;
	screen_x = get_screen_x(canvas, camera, relative_angle);
	new_size = get_size((t_get_size_config){
			camera, centered_bill_coords,
			image->size, canvas->size, relative_angle});
	if (screen_x + get_image_width(bill->sprites[(int)ft_angle_distance(camera->entity->coords, centered_bill_coords)]) * 0.7 < 0 || screen_x - get_image_width(bill->sprites[(int)ft_angle_distance(camera->entity->coords, centered_bill_coords)]) * 0.7 > canvas->size.width)
		return ;
	render_billboard_slices((t_render_billboard_slices_config){new_size,
		screen_x, canvas, image, camera, centered_bill_coords});
}

void	render_billboards(t_game *game, t_ftm_image *canvas, t_camera *camera)
{
	int	i;

	ft_strvorder((void *)game->billboards, &camera->entity->coords,
		(void *)cmp_billboards);
	i = -1;
	while (game->billboards[++i])
		render_billboard((t_billboard *)game->billboards[i], canvas, camera);
}
