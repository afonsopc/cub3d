/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:52:09 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/29 00:07:43 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	ftm_handle_hooks(t_ftm_window *window)
{
	SDL_Event event;	

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
			window_key_down_hook(event.key.keysym.sym, window);
		else if (event.type == SDL_KEYUP)
			window_key_up_hook(event.key.keysym.sym, window);
		else if (event.type == SDL_QUIT)
			window_exit_hook(window);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			window_mouse_down_hook(event.button.button, event.button.x, event.button.y, window);
		else if (event.type == SDL_MOUSEBUTTONUP)
			window_mouse_up_hook(event.button.button, event.button.x, event.button.y, window);
		else if (event.type == SDL_MOUSEMOTION)
			window_mouse_hook(event.motion.x, event.motion.y, window);
	}
	window_loop_hook(window);
}

void	ftm_window_loop(t_ftm_window *window)
{
	while (window->running)
	{
		ftm_handle_hooks(window);
		SDL_Delay(1);
	}
}

void	ftm_window_wipe(t_ftm_window *window)
{
	mlx_clear_window(window->display, window->win);
}
