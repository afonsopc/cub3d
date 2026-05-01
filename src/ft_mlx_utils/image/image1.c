/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:51:11 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/01 16:35:46 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	ftm_image_clear(t_ftm_image *image)
{
	ftm_draw_rectangle(image, (t_coords){0, 0, 0},
		image->size, (t_ftm_rectangle){0, 0, (t_size){0, 0}});
	image->texture_dirty = true;
}

char	*ftm_image_to_str(t_ftm_image *image)
{
	if (!image)
		return (NULL);
	return (ft_strf("Image<path: \"%s\", size: {width: %d, height: %d}>",
			image->path, image->size.width, image->size.height));
}

static void	ensure_texture(t_ftm_window *window, t_ftm_image *image)
{
	if (!image->texture || image->renderer != window->display)
	{
		if (image->texture)
			SDL_DestroyTexture(image->texture);
		image->texture = SDL_CreateTexture(window->display,
				image->surface->format->format,
				SDL_TEXTUREACCESS_STREAMING,
				image->size.width, image->size.height);
		if (!image->texture)
			return ;
		SDL_SetTextureBlendMode(image->texture, SDL_BLENDMODE_BLEND);
		image->renderer = window->display;
		image->texture_dirty = true;
	}
	if (image->texture_dirty)
	{
		SDL_UpdateTexture(image->texture, NULL,
			image->surface->pixels, image->surface->pitch);
		image->texture_dirty = false;
	}
}

void	ftm_put_image_to_window(t_ftm_window *window, t_ftm_image *image,
			t_coords coords)
{
	SDL_Rect dst_rect;

	if (!image || !window || !image->surface || !window->display)
		return ;
	ensure_texture(window, image);
	if (!image->texture)
		return ;
	dst_rect.x = coords.x;
	dst_rect.y = coords.y;
	dst_rect.w = image->size.width;
	dst_rect.h = image->size.height;
	SDL_RenderCopy(window->display, image->texture, NULL, &dst_rect);
}

void	ftm_put_image_to_window_pitc(t_ftm_window *window, t_ftm_image *image,
			t_ftm_pitc_config pitc)
{
	SDL_Rect	src_rect;
	SDL_Rect	dst_rect;

	if (!image || !window || !window->display)
		return ;
	ensure_texture(window, image);
	if (!image->texture)
		return ;
	if (pitc.crop)
	{
		src_rect.x = (int)pitc.crop_start.x;
		src_rect.y = (int)pitc.crop_start.y;
		src_rect.w = (int)(pitc.crop_end.x - pitc.crop_start.x);
		src_rect.h = (int)(pitc.crop_end.y - pitc.crop_start.y);
	}
	else
	{
		src_rect.x = 0;
		src_rect.y = 0;
		src_rect.w = image->size.width;
		src_rect.h = image->size.height;
	}
	dst_rect.x = (int)pitc.coords.x;
	dst_rect.y = (int)pitc.coords.y;
	if (pitc.resize)
	{
		dst_rect.w = pitc.size.width;
		dst_rect.h = pitc.size.height;
	}
	else
	{
		dst_rect.w = src_rect.w;
		dst_rect.h = src_rect.h;
	}
	SDL_RenderCopy(window->display, image->texture, &src_rect, &dst_rect);
}
