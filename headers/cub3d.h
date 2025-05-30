/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:14:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/30 20:00:08 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define PI 3.14159265359

// MLX
# include <mlx.h>

// Internal Libs
# include <ft_error.h>
# include <ft_utils.h>
# include <ft_mlx_utils.h>
# include <ft_threads.h>
# include <ft_audio.h>

// External Libs
# include <X11/keysym.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>

// Window Config
# define W_TITLE "Wolfenstein"
# define W_WIDTH 1024
# define W_HEIGHT 768

// Default minimap size multipliers
# define MINIMAP_WIDTH_MULTIPLIER 0.25
# define MINIMAP_HEIGHT_MULTIPLIER 0.25

// Default Font
# define DEFAULT_FONT_PATH "assets/fonts/IBM-VGA-8x16"

// Placeholder Image
# define PLACEHOLDER_IMAGE_PATH "assets/textures/placeholder.xpm"
# define PLACEHOLDER_SPRITE_UPDATE_DELAY 0

// Player Config
# define PLAYER_RAYS_NO_HIT_LENGTH 50.0
# define PLAYER_FOV 73.5
# define PLAYER_RAYS W_WIDTH
# define PLAYER_RAY_SUBRAYS 5
# define PLAYER_HITBOX_RADIUS 0.23
# define PLAYER_MOUSE_LOOK_VELOCITY 30.0
# define PLAYER_KEY_LOOK_VELOCITY 90.0
# define PLAYER_WALK_VELOCITY 3.0
# define PLAYER_SPRINT_VELOCITY 5.0
# define PLAYER_RAY_HIT_ENTITIES_NUMBER 5
# define PLAYER_MAX_TARGET_DISTANCE 1.2

// Door Config
# define DOOR_ANIMATION_DURATION 10.0
# define DOOR_ANIMATION_FRAMES 60

// Map Config
# define DEFAULT_AIR_TYPES "0 \t\n\v\f\r"
# define DEFAULT_WALL_TYPES "1"
# define DEFAULT_PLAYER_TYPES "NSEW"

// Raytracing Threads
# define CAMERA_THREADS 4

// Delta Time Config
# define DELTA_TIME_START 0.016f

// FPS Config
# define FPS_LIMIT 1000

typedef struct s_sprite
{
	t_list			*images;
	ssize_t			index;
	t_time			updated_at;
	t_time			update_delay;
	bool			reversed;
	bool			running;
	bool			loop;
}	t_sprite;

typedef enum e_entity_type
{
	ENTITY,
	ENTITY_PLAYER,
	ENTITY_WALL,
	ENTITY_DOOR,
	ENTITY_ENEMY,
	ENTITY_BILLBOARD,
}	t_entity_type;

typedef struct s_entity
{
	void			(*frame)(struct s_entity *this, double delta_time);
	void			(*free)(void *this);
	void			(*action)(struct s_entity *entity,
			struct s_entity *actioner);
	bool			transparent;
	int				max_health;
	int				health;
	bool			hard;
	bool			wall;
	bool			billboard;
	char			identifier;
	t_coords		coords;
	t_size			size;
	t_entity_type	type;
	struct s_entity	*target_entity;
	t_direction		target_entity_direction;
}	t_entity;

typedef struct s_camera
{
	t_entity		*entity;
	double			fov;
	unsigned int	rays;
	double			*ray_distances;
}	t_camera;

typedef struct s_billboard
{
	t_entity	entity;
	t_sprite	*sprites[360];
}	t_billboard;

typedef struct s_enemy
{
	t_billboard	billboard;
	t_sprite	*idle_sprite;
}	t_enemy;

typedef struct s_player
{
	t_billboard	billboard;
	bool		walking_forward;
	bool		walking_left;
	bool		walking_backward;
	bool		walking_right;
	bool		looking_right;
	bool		looking_left;
	bool		sprinting;
	bool		action;
	bool		already_actioned;
	double		mouse_moviment;
	double		mouse_look_velocity;
	double		key_look_velocity;
	double		walk_velocity;
	double		sprint_velocity;
	t_fta_audio	*collision_sound;
}	t_player;

typedef struct s_wall
{
	t_entity	entity;
	t_sprite	*north_sprite;
	t_sprite	*south_sprite;
	t_sprite	*west_sprite;
	t_sprite	*east_sprite;
}	t_wall;

typedef struct s_door
{
	t_wall		wall;
	t_direction	direction;
	t_sprite	opening_sprite;
	t_sprite	*door_sprite;
	bool		opened;
	int			last_animation_index;
	t_fta_audio	*open_sound;
	t_fta_audio	*close_sound;
}	t_door;

typedef struct s_identifiers
{
	t_list	*wall;
	t_list	*player;
	t_list	*door;
	t_list	*enemy;
	t_list	*billboard;
	t_list	*air;
}	t_identifiers;

typedef struct s_map
{
	char			*path;
	t_hashmap		*types;
	char			**raw;
	char			**map;
	t_size			size;
	t_identifiers	identifiers;
}	t_map;

typedef struct s_environment
{
	unsigned int	floor_color;
	unsigned int	ceiling_color;
}	t_environment;

typedef struct s_hud_minimap
{
	bool			enabled;
	t_coords		coords;
	t_size			size;
	bool			full;
	double			width_multiplier;
	double			height_multiplier;
	unsigned int	background_color;
	unsigned int	border_color;
	unsigned int	entity_color;
	unsigned int	door_color;
	unsigned int	wall_color;
	unsigned int	player_color;
	unsigned int	player_ray_color;
	unsigned int	player_middle_ray_color;
	double			zoom_level;
}	t_hud_minimap;

typedef struct s_hud_debug
{
	t_ftm_font	*font;
	char		*fps;
	char		*fps_min;
	char		*fps_max;
	char		*fps_limit;
	char		*target_x;
	char		*target_y;
	char		*target_yaw;
	char		*target_type;
	char		*target_id;
	char		*player_x;
	char		*player_y;
	char		*player_health;
	char		*player_yaw;
	char		*player_fov;
	char		*entities_count;
	bool		enabled;
}	t_hud_debug;

typedef struct s_hud_stats
{
	t_ftm_font	*font;
	t_sprite	*sprite;
	t_list		*states_list;
	int			states;
	int			prev_health;
	bool		enabled;
}	t_hud_stats;

typedef struct s_hud_action
{
	t_ftm_font	*font;
	bool		enabled;
}	t_hud_action;

typedef struct s_hud
{
	t_entity		*entity;
	t_hud_debug		debug;
	t_hud_stats		stats;
	t_hud_action	action;
	t_hud_minimap	minimap;
	bool			enabled;
}	t_hud;

typedef struct s_fps
{
	t_time	beginning;
	t_time	fps_update_time;
	t_time	last_frame_time;
	double	delta_time;
	int		fps_limit;
	int		frame_count;
	int		fps;
	int		max;
	int		min;
}	t_fps;

typedef struct s_game
{
	t_fps				fps;
	t_hud				hud;
	t_environment		environment;
	t_hashmap			*fonts;
	t_hashmap			*sounds;
	t_map				*map;
	t_camera			camera;
	t_player			*player;
	t_list				*entities;
	t_hashmap			*sprites;
	t_entity			***walls;
	t_entity			**billboards;
	t_ftt_thread		*camera_threads[CAMERA_THREADS];
}	t_game;

typedef struct s_cub3d
{
	t_sprite			placeholder;
	t_map				*curr_map;
	t_ftm_window		window;
	t_game				game;
}	t_cub3d;

// cub3d
t_cub3d		*cub3d(void);
void		cub3d_exit(int code);

// Game
void		game_load_map_e(t_game *game, t_ftm_window *window, t_map *map);
void		game_start(t_game *game, t_ftm_window *window);
void		clear_game(void *game);
void		free_game(void *game);
void		render_game(t_ftm_window *window, t_game *game);

// Map
t_map		*parse_map_e(char *path);
void		clear_map(void *map);
void		free_map(t_map *map);

// Loop
void		loop(void);
void		key_hook(int key, bool down);
void		mouse_hook(t_coords coords);

// Sprites
t_ftm_image	*get_sprite_image(t_sprite *sprite);
t_sprite	*get_entity_sprite(t_entity *entity, t_direction direction);
void		free_sprite(void *data);
void		clear_sprite(void *data);
void		init_sprite(t_sprite *sprite, t_list *images, t_time update_delay);
t_sprite	*sprite_new(t_list *images, t_time update_delay);
void 		insert_door_frames(t_game *game);

// Entities
void		call_entity_frames(t_list *entities, t_fps *fps);
bool		entity_x_is_transparent(t_entity *entity, t_direction direction, double x);
t_player	*player_new(t_game *game, t_ftm_window *window, char identifier);
t_wall		*wall_new(t_game *game, t_ftm_window *window, char identifier);
t_door		*door_new_e(t_game *game, t_ftm_window *window, char identifier);
t_enemy		*enemy_new(t_game *game, t_ftm_window *window, char identifier);
t_billboard	*billboard_new(t_game *game, t_ftm_window *window, char identifier);

#endif
