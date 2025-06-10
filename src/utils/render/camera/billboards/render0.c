/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:47:16 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/10 18:08:48 by afpachec         ###   ########.fr       */
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

	distance = ft_distance(gsc.camera->character->billboard.entity.coords, gsc.bill_coords);
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

static void	render_billboard(t_billboard *bill, t_ftm_image *canvas,
	t_camera *camera)
{
	t_ftm_image	*image;
	t_size		new_size;
	double		screen_x;
	double		relative_angle;

	if (!bill->entity.active)
		return ;
	relative_angle = get_relative_angle(camera->character->billboard.entity.coords,
			bill->entity.coords);
	image = get_sprite_image(bill->sprites[((int)ft_angle_distance(bill->entity.coords, camera->character->billboard.entity.coords)) % 359]);
	if (!image)
		return ;
	screen_x = get_screen_x(canvas, camera, relative_angle);
	new_size = get_size((t_get_size_config){
			camera, bill->entity.coords,
			image->size, canvas->size, relative_angle});
	if (screen_x + image->size.width * 0.7 < 0 || screen_x - image->size.width * 0.7 > canvas->size.width)
		return ;
	render_billboard_slices((t_render_billboard_slices_config){new_size,
		screen_x, canvas, image, camera, bill, bill->entity.coords});
}

void	render_billboards(t_game *game, t_ftm_image *canvas, t_camera *camera)
{
	int	i;

	ft_strvorder((void *)game->billboards, &camera->character->billboard.entity.coords,
		(void *)cmp_billboards);
	i = -1;
	while (game->billboards[++i])
		if (game->billboards[i] != (t_entity *)camera->character)
			render_billboard((t_billboard *)game->billboards[i], canvas, camera);
}
