/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:38:00 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/14 15:06:22 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

int	window_loop_hook(void *window)
{
	if (((t_ftm_window *)window)->loop_hook)
		((t_ftm_window *)window)->loop_hook();
	return (0);
}

int	window_key_down_hook(int key, void *window)
{
	if (((t_ftm_window *)window)->key_hook)
		((t_ftm_window *)window)->key_hook(key, true);
	return (0);
}

int	window_key_up_hook(int key, void *window)
{
	if (((t_ftm_window *)window)->key_hook)
		((t_ftm_window *)window)->key_hook(key, false);
	return (0);
}

int	window_exit_hook(void *window)
{
	if (((t_ftm_window *)window)->exit_hook)
		((t_ftm_window *)window)->exit_hook(0);
	return (0);
}
