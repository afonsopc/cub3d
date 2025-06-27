/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   billboards.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:46:51 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/08 13:53:59 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BILLBOARDS_H
# define BILLBOARDS_H 

# include "../camera.h"

typedef struct s_get_size_config
{
	t_camera	*camera;
	t_coords	bill_coords;
	t_size		bill_image_size;
	t_size		canvas_size;
	double		relative_angle;
}	t_get_size_config;

typedef struct s_render_billboard_slices_config
{
	t_size		new_size;
	double		screen_x;
	t_ftm_image	*canvas;
	t_ftm_image	*image;
	t_camera	*camera;
	t_billboard	*bill;
	t_coords	bill_coords;
}	t_render_billboard_slices_config;

void	render_billboard_slices(t_render_billboard_slices_config rbsc);
bool	cmp_billboards(void *data, t_billboard *a, t_billboard *b);

#endif