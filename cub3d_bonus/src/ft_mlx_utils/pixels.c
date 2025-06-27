/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:05:31 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/26 21:44:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_mlx_utils.h>

unsigned int	*ftm_image_pixel(t_ftm_image *image, t_coords coords)
{
	int	x;
	int	y;

	x = coords.x;
	y = coords.y;
	if (x < 0 || y < 0 || x >= image->size.width || y >= image->size.height)
		return (NULL);
	return (image->data + (y * image->size_line)
		+ (x * (image->bits_per_pixel / 8)));
}

unsigned int	ftm_blend_pixel(unsigned int dst, unsigned int src)
{
	t_rgba			src_rgba;
	t_rgba			out_rgba;
	t_rgba			dst_rgba;
	unsigned char	alpha;

	alpha = (src >> 24) & 0xFF;
	if (!alpha)
		return (dst);
	if (alpha == 255)
		return (src);
	src_rgba = (t_rgba){(src >> 16) & 0xFF, (src >> 8) & 0xFF, src & 0xFF, 0};
	dst_rgba = (t_rgba){(dst >> 16) & 0xFF, (dst >> 8) & 0xFF, dst & 0xFF, 0};
	out_rgba = (t_rgba){(src_rgba.r * alpha + dst_rgba.r * (255 - alpha)) / 255,
		(src_rgba.g * alpha + dst_rgba.g * (255 - alpha)) / 255,
		(src_rgba.b * alpha + dst_rgba.b * (255 - alpha)) / 255, 0};
	return ((0xFF << 24) | (out_rgba.r << 16) | (out_rgba.g << 8) | out_rgba.b);
}

unsigned int	ftm_remove_pixel_transparency(unsigned int value)
{
	return (value - (value / 0x01000000) + 0xFF000000);
}

void	ftm_set_pixel(unsigned int *pixel, unsigned int value)
{
	if (!pixel)
		return ;
	if (value / 0x01000000 == 0xFF)
		*pixel = value;
	else
		*pixel = ftm_blend_pixel(*pixel, value);
}
