/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pitc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:48:02 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/01 16:40:41 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	ftm_put_image_to_canvas(t_ftm_image *canvas, t_ftm_image *image,
		t_ftm_pitc_config pitc)
{
	SDL_Rect		src_rect;
	SDL_Rect		dst_rect;

	if (!canvas || !image || !canvas->surface || !image->surface)
		return ;
	if (pitc.crop)
	{
		src_rect.x = (int)pitc.crop_start.x;
		src_rect.y = (int)pitc.crop_start.y;
		src_rect.w = (int)(pitc.crop_end.x - pitc.crop_start.x);
		src_rect.h = (int)(pitc.crop_end.y - pitc.crop_start.y);
		if (src_rect.x < 0 || src_rect.y < 0 || 
			src_rect.x + src_rect.w > image->size.width ||
			src_rect.y + src_rect.h > image->size.height ||
			src_rect.w <= 0 || src_rect.h <= 0)
			return ;
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
		pthread_mutex_lock(&canvas->mutex);
		SDL_BlitScaled(image->surface, &src_rect, canvas->surface, &dst_rect);
		pthread_mutex_unlock(&canvas->mutex);
	}
	else
	{
		dst_rect.w = src_rect.w;
		dst_rect.h = src_rect.h;
		if (dst_rect.w <= 0 || dst_rect.h <= 0)
			return ;
		pthread_mutex_lock(&canvas->mutex);
		SDL_BlitSurface(image->surface, &src_rect, canvas->surface, &dst_rect);
		pthread_mutex_unlock(&canvas->mutex);
	}
}
