/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:58:13 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 16:18:39 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	ftm_init_window_e(t_ftm_window *window, t_size size, char *title)
{
	fte_set(NULL);
	window->display = mlx_init();
	if (!window->display)
		return (fte_set("init mlx"));
	window->win = mlx_new_window(window->display,
			size.width, size.height, title);
	if (!window->win)
		return (mlx_destroy_display(window->display),
			fte_set("init window"));
	window->size = size;
	window->title = title;
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
	mlx_destroy_window(window->display, window->win);
	mlx_destroy_display(window->display);
	free(window->display);
}

void	ftm_free_window(void *data)
{
	if (!data)
		return ;
	ftm_clear_window(data);
	free(data);
}
