/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lines.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:37:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/17 13:08:19 by paude-so         ###   ########.fr       */
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

	dxy = (t_coords){end.x - start.x, end.y - start.y, 0, 0};
	step = FT_MLX_UTILS_DRAW_LINE_STEP;
	dxy.x /= step;
	dxy.y /= step;
	xy = (t_coords){start.x, start.y, 0, 0};
	i = 0;
	while (i <= step)
	{
		ftm_set_pixel(ftm_image_pixel(canvas,
				(t_coords){xy.x, xy.y, 0, 0}), color);
		xy.x += dxy.x;
		xy.y += dxy.y;
		i++;
	}
}

void	ftm_draw_line_angle(t_ftm_image *canvas, t_coords start,
	double length, unsigned int color)
{
	static t_coords	end;

	start.yaw = ft_normalize_angle(start.yaw);
	end.x = start.x + cos(start.yaw * (FT_MLX_UTILS_PI / 180.0)) * length;
	end.y = start.y + sin(start.yaw * (FT_MLX_UTILS_PI / 180.0)) * length;
	ftm_draw_line(canvas, start, end, color);
}

void	ftm_draw_rectangle(t_ftm_image *canvas, t_coords coords, t_size size,
	t_ftm_rectangle rectangle)
{
	int				i;
	int				j;
	unsigned int	tmp_color;

	i = -1;
	while (++i < size.width)
	{
		j = -1;
		while (++j < size.height)
		{
			tmp_color = rectangle.background_color;
			if (i == 0 || i == size.width - rectangle.border_size.width
				|| j == 0 || j == size.height - rectangle.border_size.height)
				tmp_color = rectangle.border_color;
			ftm_set_pixel(ftm_image_pixel(canvas,
					(t_coords){coords.x + i, coords.y + j, 0, 0}), tmp_color);
		}
	}
}
