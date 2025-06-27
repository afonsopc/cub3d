/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:07:13 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/18 20:41:39 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"

void	render_debug_entities(t_game *game, t_ftm_image *canvas,
	t_coords coords)
{
	char	*str;

	coords.x += render_debug_str(game, canvas, "Entities", coords) + 5;
	str = ft_strf("%d", ft_list_size(game->entities));
	render_debug_str(game, canvas, str, coords);
	free(str);
}
