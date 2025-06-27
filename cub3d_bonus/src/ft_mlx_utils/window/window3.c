/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 22:21:31 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/27 16:15:29 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	ftm_window_resize_e(t_ftm_window *window, t_size size)
{
	char	*title;

	title = window->title;
	mlx_destroy_window(window->display, window->win);
	window->win = mlx_new_window(window->display,
			size.width, size.height, title);
	window->size = size;
	ftm_update_hooks(window);
}

t_size	ftm_window_toggle_fullscreen(t_ftm_window *window, t_size prev_size)
{
	t_size	new_size;

	window->fullscreen = !window->fullscreen;
	new_size = prev_size;
	if (window->fullscreen)
		new_size = ftm_get_screen_size(window);
	ftm_window_resize_e(window, new_size);
	ftm_window_notify_fullscreen(window);
	return (new_size);
}

t_size	ftm_get_screen_size(t_ftm_window *window)
{
	return ((t_size){XDisplayWidth(((t_xvar *)window->display)->display,
			((t_xvar *)window->display)->screen),
		XDisplayHeight(((t_xvar *)window->display)->display,
			((t_xvar *)window->display)->screen)});
}

void	ftm_window_notify_fullscreen(t_ftm_window *window)
{
	XEvent	xev;
	Display	*display;

	display = ((t_xvar *)window->display)->display;
	ft_bzero(&xev, sizeof(XEvent));
	xev.type = ClientMessage;
	xev.xclient.window = ((t_win_list *)window->win)->window;
	xev.xclient.message_type = XInternAtom(display, "_NET_WM_STATE",
			False);
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = window->fullscreen;
	xev.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN",
			False);
	XSendEvent(display, DefaultRootWindow(display), False,
		SubstructureRedirectMask | SubstructureNotifyMask, &xev);
	XFlush(display);
}
