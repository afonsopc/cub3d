/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:05:31 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/14 16:42:42 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_mlx_utils.h>

unsigned int	*ftm_image_pixel(t_ftm_image *image, t_coords coords)
{
	int	x;
	int	y;

	x = coords.x;
	y = coords.y;
	if (x < 0 || y < 0 || x > image->size.width || y > image->size.height)
		return (NULL);
	return (image->data + (y * image->size_line)
		+ (x * (image->bits_per_pixel / 8)));
}

void	ftm_set_pixel(unsigned int *pixel, unsigned int value)
{
	if (!pixel)
		return ;
	*pixel = value;
}
