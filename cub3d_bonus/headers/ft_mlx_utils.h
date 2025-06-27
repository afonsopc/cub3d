/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mlx_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:46:27 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 16:17:44 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MLX_UTILS_H
# define FT_MLX_UTILS_H

// External Libs
# include <mlx.h>
# include <X11/X.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <SDL2/SDL.h>

// Internal Libs
# include <ft_utils.h>
# include <ft_error.h>

// Line drawing step size (Used for diagonal lines)
# define FTM_DRAW_LINE_STEP 1024

# define FTM_MOUSE_LEFT 1
# define FTM_MOUSE_RIGHT 3
# define FTM_MOUSE_MIDDLE 2
# define FTM_MOUSE_SCROLL_UP 4
# define FTM_MOUSE_SCROLL_DOWN 5

# define FTM_GAMEPAD_RSTICK -2002
# define FTM_GAMEPAD_LSTICK -2001
# define FTM_GAMEPAD_R2 -1002
# define FTM_GAMEPAD_L2 -1001
# define FTM_GAMEPAD_R1 -11
# define FTM_GAMEPAD_L1 -10
# define FTM_GAMEPAD_R3 -9
# define FTM_GAMEPAD_L3 -8
# define FTM_GAMEPAD_A -1
# define FTM_GAMEPAD_B -2
# define FTM_GAMEPAD_X -3
# define FTM_GAMEPAD_Y -4
# define FTM_GAMEPAD_RIGHT -15
# define FTM_GAMEPAD_LEFT -14
# define FTM_GAMEPAD_DOWN -13
# define FTM_GAMEPAD_UP -12
# define FTM_GAMEPAD_MENU -7
# define FTM_GAMEPAD_SELECT -5
# define FTM_GAMEPAD_SHARE -16
# define FTM_GAMEPAD_MAIN -6

typedef struct s_ftm_image				t_ftm_image;
typedef struct s_ftm_key_hook_values	t_ftm_key_hook_values;
typedef struct s_ftm_controller			t_ftm_controller;
typedef struct s_ftm_window				t_ftm_window;
typedef struct s_ftm_font				t_ftm_font;
typedef struct s_ftm_pitc_config		t_ftm_pitc_config;
typedef struct s_ftm_text_config		t_ftm_text_config;
typedef struct s_ftm_rectangle			t_ftm_rectangle;
typedef struct s_win_list				t_win_list;
typedef struct s_xvar					t_xvar;

struct s_ftm_image
{
	void			*display;
	char			*path;
	void			*img_ptr;
	int				bits_per_pixel;
	int				size_line;
	int				endian;
	void			*data;
	t_size			size;
};

struct s_ftm_key_hook_values
{
	int					key;
	t_coords			coords;
	double				pressure;
	bool				down;
	t_ftm_controller	*controller;
};

struct s_ftm_controller
{
	SDL_GameController	*controller;
	void				(*key_hook)(t_ftm_key_hook_values);
	int					id;
	int					joy_id;
	int16_t				lx;
	int16_t				ly;
	int16_t				rx;
	int16_t				ry;
};

struct s_ftm_window
{
	t_size				size;
	void				*win;
	void				*display;
	t_list				*controllers;
	bool				using_mouse;
	bool				fullscreen;
	char				*title;
	void				(*loop_hook)(void);
	void				(*key_hook)(t_ftm_key_hook_values);
	void				(*exit_hook)(int code);
	void				(*mouse_hook)(t_coords coords);
};

struct s_ftm_font
{
	char		*dir;
	t_ftm_image	*characters[255];
};

struct s_ftm_pitc_config
{
	t_coords	coords;
	bool		crop;
	t_coords	crop_start;
	t_coords	crop_end;
	bool		resize;
	t_size		size;
	void		*pixel_modifier_data;
	unsigned	(*pixel_modifier)(void *data, unsigned pixel);
};

struct s_ftm_text_config
{
	char			*text;
	t_coords		coords;
	int				height;
	int				spacing;
	unsigned int	color;
};

struct s_ftm_rectangle
{
	unsigned int	background_color;
	unsigned int	border_color;
	t_size			border_size;
};

struct s_win_list
{
	Window				window;
	GC					gc;
	struct s_win_list	*next;
	int					(*mouse_hook)();
	int					(*key_hook)();
	int					(*expose_hook)();
	void				*mouse_param;
	void				*key_param;
	void				*expose_param;
};

struct s_xvar
{
	Display		*display;
	Window		root;
	int			screen;
	int			depth;
	Visual		*visual;
	Colormap	cmap;
	int			private_cmap;
	t_win_list	*win_list;
	int			(*loop_hook)();
	void		*loop_param;
	int			use_xshm;
	int			pshm_format;
	int			do_flush;
	int			decrgb[6];
	Atom		wm_delete_window;
	Atom		wm_protocols;
	int			end_loop;
};

void				ftm_controller_init_e(t_ftm_controller *controller, int id);
void				ftm_controller_clear(void *data);
t_ftm_controller	*ftm_controller_new(int id);
void				ftm_controller_free(void *data);
t_list				*ftm_load_controllers(void);
void				ftm_controller_event_handler(void *userdata,
						SDL_Event *event);
void				ftm_call_controllers_event_handlers(t_list *controllers);

void				ftm_free_image(void *image);
t_ftm_image			*ftm_image_from_file(t_ftm_window *window, char *path);
t_ftm_image			*ftm_image_new(t_ftm_window *window, t_size size);
t_list				*ftm_images_from_files(t_ftm_window *window,
						char **file_paths);

void				ftm_image_clear(t_ftm_image *image);
void				ftm_put_image_to_canvas(t_ftm_image *canvas,
						t_ftm_image *image, t_ftm_pitc_config pitc);

char				*ftm_image_to_str(t_ftm_image *image);
void				ftm_put_image_to_window(t_ftm_window *window,
						t_ftm_image *image, t_coords coords);
void				ftm_put_image_to_window_pitc(t_ftm_window *window,
						t_ftm_image *image,
						t_ftm_pitc_config pitc);

t_ftm_font			*ftm_font_new(t_ftm_window *window, const char *dir);
void				ftm_clear_font(void *font);
void				ftm_free_font(void *font);

void				ftm_draw_text(t_ftm_image *canvas, t_ftm_font *font,
						t_ftm_text_config text_config);
int					ftm_get_text_size_prediction(t_ftm_font *font,
						t_ftm_text_config text_config);
void				ftm_draw_text_in_box(t_ftm_image *canvas, t_ftm_font *font,
						t_ftm_text_config text_config,
						t_ftm_rectangle rectangle);

void				ftm_clear_window(void *window);
void				ftm_free_window(void *window);
t_ftm_window		*ftm_window_new_e(t_size size, char *title);
void				ftm_init_window_e(t_ftm_window *window, t_size size,
						char *title);
void				ftm_update_hooks(t_ftm_window *window);
void				ftm_window_loop(t_ftm_window *window);
void				ftm_window_wipe(t_ftm_window *window);

void				ftm_window_resize_e(t_ftm_window *window, t_size size);
t_size				ftm_window_toggle_fullscreen(t_ftm_window *window,
						t_size prev_size);
t_size				ftm_get_screen_size(t_ftm_window *window);
void				ftm_window_notify_fullscreen(t_ftm_window *window);

void				ftm_window_update_controllers_hooks(t_ftm_window *window);
void				ftm_window_reload_controllers(t_ftm_window *window);

unsigned int		*ftm_image_pixel(t_ftm_image *image, t_coords coords);
unsigned int		ftm_remove_pixel_transparency(unsigned int value);
unsigned int		ftm_blend_pixel(unsigned int dst, unsigned int src);
void				ftm_set_pixel(unsigned int *pixel, unsigned int value);

void				ftm_draw_line(t_ftm_image *canvas, t_coords start,
						t_coords end, unsigned int color);
void				ftm_draw_line_angle(t_ftm_image *canvas, t_coords start,
						double length, unsigned int color);
void				ftm_draw_rectangle(t_ftm_image *canvas, t_coords coords,
						t_size size, t_ftm_rectangle rectangle);
void				ftm_draw_arrow(t_ftm_image *canvas, t_coords coords,
						t_size size, unsigned int color);

#endif
