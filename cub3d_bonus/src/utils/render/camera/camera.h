/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:40:06 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/24 00:47:43 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "../render.h"

void	render_walls(t_game *game, t_ftm_image *canvas, t_camera *camera);
void	render_billboards(t_game *game, t_ftm_image *canvas, t_camera *camera);

#endif
