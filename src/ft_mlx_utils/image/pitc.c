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

void	ftm_blit_column_fast(t_ftm_image *canvas, t_ftm_image *image,
	t_ftm_blit_column_args a)
{
	SDL_Surface		*src;
	SDL_Surface		*dst;
	Uint32			*src_col_base;
	Uint32			*dst_px;
	Uint32			colorkey;
	int				has_ck;
	int				dst_stride;
	int				src_stride;
	int				src_h;
	int				dy_start;
	int				dy_end;
	int				dx_start;
	int				dx_end;
	int				src_y_step;
	int				src_y;
	int				dx;
	int				dy;
	int				sy;
	Uint32			px;

	if (!canvas || !canvas->surface || !image || !image->surface)
		return ;
	src = image->surface;
	dst = canvas->surface;
	if (a.dst_h <= 0 || a.dst_w <= 0
		|| src->format->BytesPerPixel != 4
		|| dst->format->BytesPerPixel != 4)
		return ;
	src_h = src->h;
	if (a.src_col < 0)
		a.src_col = 0;
	if (a.src_col >= src->w)
		a.src_col = src->w - 1;
	dy_start = a.dst_y < 0 ? 0 : a.dst_y;
	dy_end = a.dst_y + a.dst_h;
	if (dy_end > dst->h)
		dy_end = dst->h;
	dx_start = a.dst_x < 0 ? 0 : a.dst_x;
	dx_end = a.dst_x + a.dst_w;
	if (dx_end > dst->w)
		dx_end = dst->w;
	if (dy_start >= dy_end || dx_start >= dx_end)
		return ;
	has_ck = (SDL_GetColorKey(src, &colorkey) == 0);
	colorkey &= 0x00FFFFFF;
	src_stride = src->pitch / 4;
	dst_stride = dst->pitch / 4;
	src_col_base = (Uint32 *)src->pixels + a.src_col;
	dst_px = (Uint32 *)dst->pixels;
	src_y_step = (src_h << 16) / a.dst_h;
	dx = dx_start;
	while (dx < dx_end)
	{
		src_y = (dy_start - a.dst_y) * src_y_step;
		dy = dy_start;
		while (dy < dy_end)
		{
			sy = src_y >> 16;
			if (sy < 0)
				sy = 0;
			else if (sy >= src_h)
				sy = src_h - 1;
			px = src_col_base[sy * src_stride];
			if ((px & 0xFF000000)
				&& (!has_ck || (px & 0x00FFFFFF) != colorkey))
				dst_px[dy * dst_stride + dx] = px;
			src_y += src_y_step;
			dy++;
		}
		dx++;
	}
	canvas->texture_dirty = true;
}

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
	canvas->texture_dirty = true;
}
