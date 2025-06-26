/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:14:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 15:20:56 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define PI 3.14159265359

// MLX
# include <mlx.h>

// Internal Libs
# include <ft_error.h>
# include <ft_mlx_utils.h>
# include <ft_utils.h>

// External Libs
# include <X11/keysym.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// Window Config
# define W_TITLE "cub3d"
# define W_WIDTH 1024
# define W_HEIGHT 768
# define FPS60 0.01666666667

// Player Config
# define PLAYER_RAYS_NO_HIT_LENGTH 100.0
# define PLAYER_FOV 50.0
# define PLAYER_RAYS W_WIDTH
# define PLAYER_KEY_LOOK_VELOCITY 2.0
# define PLAYER_WALK_VELOCITY 4.0

// Map Config
# define DEFAULT_AIR_TYPES "0 \t\n\v\f\r"
# define DEFAULT_WALL_TYPES "1"
# define DEFAULT_PLAYER_TYPES "NSEW"
# define DEFAULT_MAP_TYPES "01NSEW "

typedef struct s_sprite
{
	t_list			*images;
	size_t			index;
	t_time			updated_at;
	t_time			update_delay;
}					t_sprite;

typedef enum e_entity_type
{
	ENTITY_PLAYER,
	ENTITY_WALL,
}					t_entity_type;

typedef struct s_entity
{
	void			(*frame)(struct s_entity *this, double delta_time);
	void			(*free)(void *this);
	bool			block;
	char			identifier;
	t_coords		coords;
	t_entity_type	type;
}					t_entity;

typedef struct s_ray
{
	double			length;
	double			angle;
	t_entity		*hit_entity;
	t_direction		hit_direction;
	double			x_of_hit_in_entity;
	int				_height;
}					t_ray;

typedef struct s_player
{
	t_entity		base;
	bool			walking_forward;
	bool			walking_left;
	bool			walking_backward;
	bool			walking_right;
	bool			looking_right;
	bool			looking_left;
	double			key_look_velocity;
	double			walk_velocity;
	t_ray			rays[PLAYER_RAYS];
}					t_player;

typedef struct s_wall
{
	t_entity		base;
	t_sprite		*north_sprite;
	t_sprite		*south_sprite;
	t_sprite		*west_sprite;
	t_sprite		*east_sprite;
}					t_wall;

typedef struct s_identifiers
{
	t_list			*wall;
	t_list			*player;
	t_list			*air;
}					t_identifiers;

typedef struct s_map
{
	char			*path;
	t_hashmap		*types;
	char			**raw;
	char			**map;
	t_size			size;
	t_identifiers	identifiers;
}					t_map;

typedef struct s_environment
{
	unsigned int	floor_color;
	unsigned int	ceiling_color;
}					t_environment;

typedef struct s_render_data
{
	t_player		*player;
	t_ftm_image		*canvas;
	int				ray_idx;
	double			width_scale;
	int				screen_x;
	int				strip_width;
	t_ftm_image		*texture;
	double			wall_height;
	int				draw_start;
	int				draw_end;
	double			tex_x;
	double			tex_y_step;
}					t_render_data;

typedef struct s_game
{
	t_environment	environment;
	t_map			*map;
	t_player		*player;
	t_list			*entities;
	t_hashmap		*sprites;
}					t_game;

typedef struct s_cub3d
{
	t_map			*curr_map;
	t_ftm_window	window;
	t_game			game;
}					t_cub3d;

void				game_load_map_e(t_game *game, t_ftm_window *window,
						t_map *map);
void				clear_game(void *game);
void				free_game(void *game);
void				render_game(t_ftm_window *window, t_game *game);
void				free_sprite(void *data);
void				clear_sprite(void *data);
void				init_sprite(t_sprite *sprite, t_list *images,
						t_time update_delay);
t_sprite			*sprite_new(t_list *images, t_time update_delay);

// cub3d
t_cub3d				*cub3d(void);
void				cub3d_exit(int code);

// Map
t_map				*parse_map_e(char *path);
void				destroy_map(t_map *map);
void				ft_backtrack_e(t_game *game);

// Loop
void				loop(void);
void				key_hook(int key, bool down);

// Render
void				render_ceiling_and_floor(t_game *game, t_ftm_image *canvas);

// Entities
void				call_entity_frames(t_list *entities, double delta_time);
t_sprite			*get_entity_sprite(t_entity *entity, t_direction direction);

// Raycasting
void				render_raycasting_mega(t_game *game, t_ftm_image *canvas);

// Sprites
t_ftm_image			*get_sprite_image(t_sprite *sprite);

// Entities
t_player			*player_new(char direction);
t_wall				*wall_new(char identifier, t_game *game);

#endif
