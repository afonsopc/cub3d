/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 22:21:31 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/29 00:17:49 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	ftm_window_resize_e(t_ftm_window *window, t_size size)
{
	SDL_SetWindowSize(window->win, size.width, size.height);
	window->size = size;
}

t_size	ftm_window_toggle_fullscreen(t_ftm_window *window, t_size prev_size)
{
	t_size	new_size;

	window->fullscreen = !window->fullscreen;
	new_size = prev_size;
	if (window->fullscreen)
	{
		SDL_SetWindowFullscreen(window->win, SDL_WINDOW_FULLSCREEN_DESKTOP);
        new_size = ftm_get_screen_size(window);
	}
	else
	{
		SDL_SetWindowFullscreen(window->win, 0);
        ftm_window_resize_e(window, prev_size);
	}
	window->size = new_size;
	return (new_size);
}

t_size	ftm_get_screen_size(t_ftm_window *window)
{
	SDL_DisplayMode	dm;
	int				display_index;
		
	display_index = SDL_GetWindowDisplayIndex(window->win);
	if (SDL_GetDisplayMode(display_index, 0, &dm) != 0)
		return ((t_size){0, 0});
	return ((t_size){dm.w, dm.h});
}

void	ftm_window_notify_fullscreen(t_ftm_window *window)
{
	(void)window;
}
