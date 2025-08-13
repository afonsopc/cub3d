/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lines.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:37:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/01 16:35:46 by afpachec         ###   ########.fr       */
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

static void	draw_rect_with_alpha(t_ftm_image *canvas, SDL_Rect rect, unsigned int color)
{
	Uint32			sdl_color;
	int				x;
	int				y;
	unsigned int	*pixel;

	if ((color >> 24) == 0xFF)
	{
		sdl_color = SDL_MapRGBA(canvas->surface->format,
				(color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 0xFF);
		SDL_FillRect(canvas->surface, &rect, sdl_color);
		return ;
	}
	y = rect.y - 1;
	while (++y < rect.y + rect.h && y < canvas->size.height)
	{
		if (y < 0)
			continue ;
		x = rect.x - 1;
		while (++x < rect.x + rect.w && x < canvas->size.width)
		{
			if (x < 0)
				continue ;
			pixel = ftm_image_pixel(canvas, (t_coords){x, y, 0});
			if (pixel)
				ftm_set_pixel(pixel, color);
		}
	}
}

void	ftm_draw_rectangle(t_ftm_image *canvas, t_coords coords, t_size size,
	t_ftm_rectangle rectangle)
{
	SDL_Rect	rect;
	SDL_Rect	border_rect;

	if (!canvas || !canvas->surface)
		return ;
	rect.x = (int)coords.x;
	rect.y = (int)coords.y;
	rect.w = size.width;
	rect.h = size.height;
	pthread_mutex_lock(&canvas->mutex);
	if ((rectangle.background_color >> 24) > 0)
		draw_rect_with_alpha(canvas, rect, rectangle.background_color);
	if ((rectangle.border_color >> 24) > 0
		&& (rectangle.border_size.width > 0 || rectangle.border_size.height > 0))
	{
		if (rectangle.border_size.height > 0)
		{
			border_rect = (SDL_Rect){rect.x, rect.y, rect.w, rectangle.border_size.height};
			draw_rect_with_alpha(canvas, border_rect, rectangle.border_color);
			border_rect = (SDL_Rect){rect.x, rect.y + rect.h - rectangle.border_size.height,
				rect.w, rectangle.border_size.height};
			draw_rect_with_alpha(canvas, border_rect, rectangle.border_color);
		}
		if (rectangle.border_size.width > 0)
		{
			border_rect = (SDL_Rect){rect.x, rect.y, rectangle.border_size.width, rect.h};
			draw_rect_with_alpha(canvas, border_rect, rectangle.border_color);
			border_rect = (SDL_Rect){rect.x + rect.w - rectangle.border_size.width, rect.y,
				rectangle.border_size.width, rect.h};
			draw_rect_with_alpha(canvas, border_rect, rectangle.border_color);
		}
	}
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
