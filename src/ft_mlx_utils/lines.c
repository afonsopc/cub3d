/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lines.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:37:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/29 21:14:37 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_mlx_utils.h>

void	ftm_draw_line(t_ftm_image *canvas, t_coords start, t_coords end,
	unsigned int color)
{
	t_coords	dxy;
	t_coords	xy;
	double		step;
	int			i;

	dxy = (t_coords){end.x - start.x, end.y - start.y, 0};
	step = FTM_DRAW_LINE_STEP;
	dxy.x /= step;
	dxy.y /= step;
	xy = (t_coords){start.x, start.y, 0};
	i = 0;
	while (i <= step)
	{
		ftm_set_pixel(ftm_image_pixel(canvas,
				(t_coords){xy.x, xy.y, 0}), color);
		xy.x += dxy.x;
		xy.y += dxy.y;
		i++;
	}
}

void	ftm_draw_line_angle(t_ftm_image *canvas, t_coords start,
	double length, unsigned int color)
{
	t_coords	end;

	start.yaw = ft_normalize_angle(start.yaw);
	end.x = start.x + ft_cos_degrees(start.yaw) * length;
	end.y = start.y + ft_sin_degrees(start.yaw) * length;
	ftm_draw_line(canvas, start, end, color);
}

void	ftm_draw_rectangle(t_ftm_image *canvas, t_coords coords, t_size size,
	t_ftm_rectangle rectangle)
{
	SDL_Renderer	*renderer;
	SDL_Rect		rect;
	SDL_Rect		border_rect;
	Uint8			r, g, b, a;

	if (!canvas || !canvas->texture)
		return ;
	renderer = (SDL_Renderer *)canvas->display;
	if (!renderer)
		return ;
	rect.x = (int)coords.x;
	rect.y = (int)coords.y;
	rect.w = size.width;
	rect.h = size.height;
	pthread_mutex_lock(&canvas->mutex);
	SDL_SetRenderTarget(renderer, canvas->texture);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	if ((rectangle.background_color >> 24) > 0)
	{
		a = (rectangle.background_color >> 24) & 0xFF;
		r = (rectangle.background_color >> 16) & 0xFF;
		g = (rectangle.background_color >> 8) & 0xFF;
		b = rectangle.background_color & 0xFF;
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderFillRect(renderer, &rect);
	}
	if ((rectangle.border_color >> 24) > 0 && 
		(rectangle.border_size.width > 0 || rectangle.border_size.height > 0))
	{
		a = (rectangle.border_color >> 24) & 0xFF;
		r = (rectangle.border_color >> 16) & 0xFF;
		g = (rectangle.border_color >> 8) & 0xFF;
		b = rectangle.border_color & 0xFF;
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		if (rectangle.border_size.height > 0)
		{
			border_rect = (SDL_Rect){rect.x, rect.y, rect.w, rectangle.border_size.height};
			SDL_RenderFillRect(renderer, &border_rect);
			border_rect = (SDL_Rect){rect.x, rect.y + rect.h - rectangle.border_size.height, 
									rect.w, rectangle.border_size.height};
			SDL_RenderFillRect(renderer, &border_rect);
		}
		if (rectangle.border_size.width > 0)
		{
			border_rect = (SDL_Rect){rect.x, rect.y, rectangle.border_size.width, rect.h};
			SDL_RenderFillRect(renderer, &border_rect);
			border_rect = (SDL_Rect){rect.x + rect.w - rectangle.border_size.width, rect.y, 
									rectangle.border_size.width, rect.h};
			SDL_RenderFillRect(renderer, &border_rect);
		}
	}
	SDL_SetRenderTarget(renderer, NULL);
	pthread_mutex_unlock(&canvas->mutex);
}

void	ftm_draw_arrow(t_ftm_image *canvas, t_coords coords, t_size size,
	unsigned int color)
{
	t_coords	end;
	t_coords	head1;
	t_coords	head2;
	double		angle;
	t_coords	head;

	coords.yaw = ft_normalize_angle(coords.yaw);
	angle = coords.yaw;
	end.x = coords.x + ft_cos_degrees(angle) * size.height;
	end.y = coords.y + ft_sin_degrees(angle) * size.height;
	head.x = size.width;
	head.yaw = 30.0;
	head1.x = end.x - ft_cos_degrees(angle - head.yaw) * head.x;
	head1.y = end.y - ft_sin_degrees(angle - head.yaw) * head.x;
	ftm_draw_line(canvas, end, head1, color);
	head2.x = end.x - ft_cos_degrees(angle + head.yaw) * head.x;
	head2.y = end.y - ft_sin_degrees(angle + head.yaw) * head.x;
	ftm_draw_line(canvas, end, head2, color);
	end.x = coords.x + ft_cos_degrees(angle) * size.height / 2;
	end.y = coords.y + ft_sin_degrees(angle) * size.height / 2;
	ftm_draw_line(canvas, end, head1, color);
	ftm_draw_line(canvas, end, head2, color);
}
