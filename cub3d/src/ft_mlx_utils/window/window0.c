/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:58:13 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/10 14:53:38 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "window.h"

void	ftm_create_window_e(t_ftm_window *window, t_size size, char *title)
{
	fte_set(ERROR_NO_ERROR);
	window->display = mlx_init();
	if (!window->display)
		return (fte_set(ERROR_INIT_MLX));
	window->win = mlx_new_window(window->display,
			size.width, size.height, title);
	if (!window->win)
		return (mlx_destroy_display(window->display),
			fte_set(ERROR_INIT_WINDOW));
	window->canvas = ftm_image_new(window, size);
	if (!window->canvas)
		return (mlx_destroy_window(window->display, window->win),
			mlx_destroy_display(window->display), fte_set(ERROR_INIT_CANVAS));
	window->size = size;
}

void	ftm_clear_window(void *window)
{
	if (!window)
		return ;
	ftm_free_image(((t_ftm_window *)window)->canvas);
	mlx_destroy_window(((t_ftm_window *)window)->display,
		((t_ftm_window *)window)->win);
	mlx_destroy_display(((t_ftm_window *)window)->display);
	free(((t_ftm_window *)window)->display);
}

void	ftm_free_window(void *window)
{
	if (!window)
		return ;
	ftm_clear_window(window);
	free(window);
}

void	ftm_update_window(t_ftm_window *window)
{
	mlx_put_image_to_window(window->display, window->win,
		window->canvas->img_ptr, 0, 0);
}
