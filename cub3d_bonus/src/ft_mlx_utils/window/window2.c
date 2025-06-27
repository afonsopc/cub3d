/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:52:09 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 16:18:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	ftm_update_hooks(t_ftm_window *window)
{
	mlx_hook(window->win, KeyPress, KeyPressMask, window_key_down_hook,
		window);
	mlx_hook(window->win, KeyRelease, KeyReleaseMask, window_key_up_hook,
		window);
	mlx_hook(window->win, DestroyNotify, KeyPressMask, window_exit_hook,
		window);
	mlx_hook(window->win, MotionNotify, PointerMotionMask, window_mouse_hook,
		window);
	mlx_hook(window->win, ButtonPress, ButtonPressMask, window_mouse_down_hook,
		window);
	mlx_hook(window->win, ButtonRelease, ButtonReleaseMask,
		window_mouse_up_hook, window);
	mlx_loop_hook(window->display, window_loop_hook, window);
	ftm_window_update_controllers_hooks(window);
}

void	ftm_window_loop(t_ftm_window *window)
{
	ftm_update_hooks(window);
	mlx_loop(window->display);
}

void	ftm_window_wipe(t_ftm_window *window)
{
	mlx_clear_window(window->display, window->win);
}
