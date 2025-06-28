/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:48:27 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/28 23:22:22 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controllers.h"

t_ftm_controller	*ftm_controller_new(int id)
{
	t_ftm_controller	*new;

	new = ft_calloc(1, sizeof(t_ftm_controller));
	if (!new)
		return (NULL);
	ftm_controller_init_e(new, id);
	if (fte_flagged())
		return (fte_set(NULL), free(new), NULL);
	return (new);
}

void	ftm_controller_free(void *data)
{
	ftm_controller_clear(data);
	free(data);
}
