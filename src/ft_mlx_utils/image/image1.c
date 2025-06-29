/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:51:11 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/29 20:27:18 by afpachec         ###   ########.fr       */
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
	SDL_Rect dst_rect;

	if (!image || !image->texture)
		return ;
	dst_rect.x = coords.x;
	dst_rect.y = coords.y;
	dst_rect.w = image->size.width;
	dst_rect.h = image->size.height;
	SDL_RenderCopy(window->display, image->texture, NULL, &dst_rect);
	SDL_RenderPresent(window->display);
}

void	ftm_put_image_to_window_pitc(t_ftm_window *window, t_ftm_image *image,
			t_ftm_pitc_config pitc)
{
	t_ftm_image	*canvas;

	if (!image)
		return ;
	canvas = ftm_image_new(window, window->size);
	if (!canvas)
		return ;
	ftm_put_image_to_canvas(canvas, image, pitc);
	ftm_put_image_to_window(window, canvas, (t_coords){0, 0, 0});
	ftm_free_image(canvas);
}
