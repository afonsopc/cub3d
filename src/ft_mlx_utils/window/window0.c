/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:58:13 by afpachec          #+#    #+#             */
/*   Updated: 2025/08/13 12:02:21 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	ftm_init_window_e(t_ftm_window *window, t_size size, char *title)
{
	fte_set(NULL);
	if (!update_sdl_usage(1))
		return (fte_set("SDL usage update failed"));
	window->win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, size.width, size.height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!window->win)
		return (update_sdl_usage(-1), fte_set("%s", SDL_GetError()));
	window->display = SDL_CreateRenderer(window->win, -1,
			SDL_RENDERER_ACCELERATED);
	if (!window->display)
		return (SDL_DestroyWindow(window->win), update_sdl_usage(-1),
			fte_set("%s", SDL_GetError()));
	window->size = size;
	window->title = title;
	window->running = true;
	ftm_window_reload_controllers(window);
}

t_ftm_window	*ftm_window_new_e(t_size size, char *title)
{
	t_ftm_window	*window;

	fte_set(NULL);
	window = ft_calloc(1, sizeof(t_ftm_window));
	if (!window)
		return (NULL);
	ftm_init_window_e(window, size, title);
	if (fte_flagged())
		return (free(window), NULL);
	return (window);
}

void	ftm_clear_window(void *data)
{
	t_ftm_window	*window;

	if (!data)
		return ;
	window = (t_ftm_window *)data;
	ft_list_destroy(&window->controllers);
	SDL_DestroyWindow(window->win);
	SDL_DestroyRenderer(window->display);
	update_sdl_usage(-1);
	ft_bzero(window, sizeof(t_ftm_window));
}

void	ftm_free_window(void *data)
{
	if (!data)
		return ;
	ftm_clear_window(data);
	free(data);
}
