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

	if (character->ray_distances_buf_cap < character->rays)
	{
		free(character->ray_distances_buf);
		character->ray_distances_buf = ft_calloc(character->rays,
				sizeof(double));
		character->ray_distances_buf_cap = character->rays
			* (character->ray_distances_buf != NULL);
	}
	if (!character->ray_distances_buf)
		return ;
	camera = (t_camera){character, character->fov, character->rays,
		character->ray_distances_buf};
	render_walls(game, canvas, &camera);
	render_billboards(game, canvas, &camera);
}
