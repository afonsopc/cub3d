/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:51:11 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 16:22:47 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	ftm_image_clear(t_ftm_image *image)
{
	ftm_draw_rectangle(image, (t_coords){0, 0, 0},
		image->size, (t_ftm_rectangle){0, 0, (t_size){0, 0}});
}

char	*ftm_image_to_str(t_ftm_image *image)
{
	if (!image)
		return (NULL);
	return (ft_strf("Image<path: \"%s\", size: {width: %d, height: %d}>",
			image->path, image->size.width, image->size.height));
}

void	ftm_put_image_to_window(t_ftm_window *window, t_ftm_image *image,
			t_coords coords)
{
	if (!image || !image->img_ptr)
		return ;
	mlx_put_image_to_window(window->display, window->win,
		image->img_ptr, coords.x, coords.y);
	mlx_do_sync(window->display);
}

void	ftm_put_image_to_window_pitc(t_ftm_window *window, t_ftm_image *image,
			t_ftm_pitc_config pitc)
{
	t_ftm_image	*canvas;

	if (!image || !image->img_ptr)
		return ;
	canvas = ftm_image_new(window, window->size);
	if (!canvas)
		return ;
	ftm_put_image_to_canvas(canvas, image, pitc);
	ftm_put_image_to_window(window, canvas, (t_coords){0, 0, 0});
	ftm_free_image(canvas);
}
