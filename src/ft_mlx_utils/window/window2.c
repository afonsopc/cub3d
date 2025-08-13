/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:52:09 by afpachec          #+#    #+#             */
/*   Updated: 2025/08/13 12:02:33 by afpachec         ###   ########.fr       */
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
		else
			ft_list_foreach(window->controllers, (void *)ftm_controller_event_handler,
				&event);
	}
	window_loop_hook(window);
}

void	ftm_window_frame(t_ftm_window *window)
{
	if (!window)
		return ;
	if (SDL_NumJoysticks() > (int)ft_list_size(window->controllers))
		ftm_window_reload_controllers(window);
	ftm_handle_hooks(window);
	ftm_window_update(window);
}

void	ftm_window_loop(t_ftm_window *window)
{
	while (window->running)
		ftm_window_frame(window);
}

void	ftm_window_wipe(t_ftm_window *window)
{
    SDL_SetRenderDrawColor(window->display, 0, 0, 0, 255);
    SDL_RenderClear(window->display);
}

void	ftm_window_update(t_ftm_window *window)
{
	if (!window || !window->display)
		return ;
	SDL_RenderPresent(window->display);
}
