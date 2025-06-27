/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:38:00 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 02:40:36 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

int	window_loop_hook(void *data)
{
	static bool		previous_using_mouse;
	t_ftm_window	*window;

	window = data;
	if (window->using_mouse != previous_using_mouse)
	{
		previous_using_mouse = window->using_mouse;
		if (previous_using_mouse)
			mlx_mouse_hide(window->display, window->win);
		else
			mlx_mouse_show(window->display, window->win);
	}
	ftm_call_controllers_event_handlers(window->controllers);
	if (window->loop_hook)
		window->loop_hook();
	return (0);
}

int	window_key_hook(t_ftm_key_hook_values key_hook_values, void *window)
{
	if (((t_ftm_window *)window)->key_hook)
		((t_ftm_window *)window)->key_hook(key_hook_values);
	return (0);
}

int	window_key_down_hook(int key, void *window)
{
	return (window_key_hook((t_ftm_key_hook_values){key, {0, 0, 0}, 1.0,
			true, NULL}, window));
}

int	window_key_up_hook(int key, void *window)
{
	return (window_key_hook((t_ftm_key_hook_values){key, {0, 0, 0}, 1.0,
			false, NULL}, window));
}

int	window_mouse_down_hook(int key, int x, int y, void *window)
{
	if (((t_ftm_window *)window)->using_mouse)
		return (window_key_hook((t_ftm_key_hook_values){key, {x, y, 0}, 1.0,
				true, NULL}, window));
	return (0);
}
