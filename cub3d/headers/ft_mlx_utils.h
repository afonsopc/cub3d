/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mlx_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:46:27 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/14 15:06:49 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MLX_UTILS_H
# define FT_MLX_UTILS_H

# define FT_MLX_UTILS_PI 3.14159265359

// External Libs
# include <X11/X.h>
# include <mlx.h>

// Internal Libs
# include <ft_error.h>
# include <ft_utils.h>

// Line drawing step size (Used for diagonal lines)
# define FT_MLX_UTILS_DRAW_LINE_STEP 1024

typedef struct s_ftm_image
{
	void			*display;
	char			*path;
	void			*img_ptr;
	int				bits_per_pixel;
	int				size_line;
	int				endian;
	void			*data;
	t_size			size;
}					t_ftm_image;

typedef struct s_ftm_window
{
	t_size			size;
	void			*win;
	void			*display;
	t_ftm_image		*canvas;
	void			(*loop_hook)(void);
	void			(*key_hook)(int key, bool down);
	void			(*exit_hook)(int code);
}					t_ftm_window;

typedef struct s_ftm_rectangle
{
	unsigned int	background_color;
	unsigned int	border_color;
	t_size			border_size;
}					t_ftm_rectangle;

void				ftm_free_image(void *image);
t_ftm_image			*ftm_image_from_file(t_ftm_window *window, char *path);
t_ftm_image			*ftm_image_new(t_ftm_window *window, t_size size);
t_list				*ftm_images_from_files(t_ftm_window *window,
						char **file_paths);

void				ftm_image_clear(t_ftm_image *image);

void				ftm_clear_window(void *window);
void				ftm_free_window(void *window);
void				ftm_create_window_e(t_ftm_window *window, t_size size,
						char *title);
void				ftm_update_window(t_ftm_window *window);
void				ftm_update_hooks(t_ftm_window *window);
void				ftm_window_loop(t_ftm_window *window);

unsigned int		*ftm_image_pixel(t_ftm_image *image, t_coords coords);
void				ftm_set_pixel(unsigned int *pixel, unsigned int value);

void				ftm_draw_line(t_ftm_image *canvas, t_coords start,
						t_coords end, unsigned int color);
void				ftm_draw_line_angle(t_ftm_image *canvas, t_coords start,
						double length, unsigned int color);
void				ftm_draw_rectangle(t_ftm_image *canvas, t_coords coords,
						t_size size, t_ftm_rectangle rectangle);

#endif