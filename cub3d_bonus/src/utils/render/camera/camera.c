/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:32:59 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/18 21:37:03 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

void	render_camera(t_game *game, t_ftm_image *canvas, t_character *character)
{
	t_camera	camera;

	camera = (t_camera){character, character->fov, character->rays,
		ft_calloc(character->rays, sizeof(double))};
	if (!camera.ray_distances)
		return ;
	render_walls(game, canvas, &camera);
	render_billboards(game, canvas, &camera);
	free(camera.ray_distances);
}
