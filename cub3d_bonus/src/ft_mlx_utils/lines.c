/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lines.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:37:16 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:57:57 by afpachec         ###   ########.fr       */
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
					(t_coords){coords.x + i, coords.y + j, 0}), tmp_color);
		}
	}
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
