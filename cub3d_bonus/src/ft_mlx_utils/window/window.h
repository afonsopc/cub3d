/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:51:04 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:55:41 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include <ft_mlx_utils.h>

int		window_loop_hook(void *window);
int		window_key_down_hook(int key, void *window);
int		window_key_up_hook(int key, void *window);
int		window_exit_hook(void *window);
int		window_mouse_hook(int x, int y, void *window);
int		window_mouse_down_hook(int key, int x, int y, void *window);
int		window_mouse_up_hook(int key, int x, int y, void *window);
int		window_key_hook(t_ftm_key_hook_values key_hook_values, void *window);

#endif