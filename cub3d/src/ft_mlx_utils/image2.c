/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:51:11 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/14 18:46:58 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_mlx_utils.h>

void	ftm_image_clear(t_ftm_image *image)
{
	ftm_draw_rectangle(image, (t_coords){0, 0, 0, 0},
		image->size, (t_ftm_rectangle){0, 0, (t_size){0, 0}});
}
