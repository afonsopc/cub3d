/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:49:29 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:49:40 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

int	window_mouse_up_hook(int key, int x, int y, void *window)
{
	if (((t_ftm_window *)window)->using_mouse)
		return (window_key_hook((t_ftm_key_hook_values){key, {x, y, 0}, 1.0,
				false, NULL}, window));
	return (0);
}

int	window_exit_hook(void *window)
{
	if (((t_ftm_window *)window)->exit_hook)
		((t_ftm_window *)window)->exit_hook(0);
	return (0);
}

int	window_mouse_hook(int x, int y, void *window)
{
	if (((t_ftm_window *)window)->mouse_hook)
		((t_ftm_window *)window)->mouse_hook((t_coords){x, y, 0});
	return (0);
}
