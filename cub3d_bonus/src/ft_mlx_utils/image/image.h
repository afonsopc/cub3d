/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:22:33 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:56:14 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMAGE_H
# define IMAGE_H

# include <ft_mlx_utils.h>

typedef struct s_pitc_data
{
	t_coords			cc;
	t_coords			sc;
	t_size				cs;
	t_ftm_pitc_config	pitc;
	t_coords			scale;
	t_size				index;
	t_ftm_image			*canvas;
	unsigned int		*cp;
	unsigned int		p;
	t_size				it_limits;
	t_ftm_image			*image;
}	t_pitc_data;

#endif