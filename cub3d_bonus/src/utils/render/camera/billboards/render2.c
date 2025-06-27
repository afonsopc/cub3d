/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:41:46 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/08 14:00:36 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "billboards.h"

bool	cmp_billboards(void *data, t_billboard *a, t_billboard *b)
{
	return (ft_distance(*((t_coords *)data), a->entity.coords)
		< ft_distance(*((t_coords *)data), b->entity.coords));
}
