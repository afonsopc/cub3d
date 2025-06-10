/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:14:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 17:02:48 by afpachec         ###   ########.fr       */
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
# define PLAYER_RAY_SUBRAYS 5
# define PLAYER_MOUSE_LOOK_VELOCITY 20.0
# define PLAYER_KEY_LOOK_VELOCITY 90.0
# define PLAYER_WALK_VELOCITY 4.0
# define PLAYER_SPRINT_VELOCITY 6.0
# define PLAYER_RAY_HIT_ENTITIES_NUMBER 5
# define PLAYER_MAX_TARGET_DISTANCE 1.2
# define PLAYER_WIDTH 0.23
# define PLAYER_HEIGHT 0.23

// Billboard Config
# define BILLBOARD_WIDTH 0.23
# define BILLBOARD_HEIGHT 0.23

// Character Config
# define CHARACTER_HIT_DELAY 100.0

// Entity Config
# define INVENTORY_SIZE 9
# define INVENTORY_SCROLL_DELAY 0.5

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

typedef struct s_sprite			t_sprite;
typedef enum e_entity_type		t_entity_type;
typedef struct s_controller		t_controller;
typedef struct s_entity			t_entity;
typedef struct s_camera			t_camera;
typedef struct s_billboard		t_billboard;
typedef struct s_player			t_player;
typedef struct s_character		t_character;
typedef struct s_wall			t_wall;
typedef struct s_cub3d			t_cub3d;
typedef struct s_game			t_game;
typedef struct s_fps			t_fps;
typedef struct s_hud			t_hud;
typedef struct s_hud_action		t_hud_action;
typedef struct s_hud_stats		t_hud_stats;
typedef struct s_hud_debug		t_hud_debug;
typedef struct s_hud_minimap	t_hud_minimap;
typedef struct s_environment	t_environment;
typedef struct s_map			t_map;
typedef struct s_door			t_door;
typedef struct s_identifiers	t_identifiers;
typedef struct s_item			t_item;
typedef struct s_food			t_food;
typedef struct s_ammo			t_ammo;
typedef struct s_drop			t_drop;
typedef t_item *(*t_item_creator)(t_game *, t_ftm_window *, char);
typedef t_entity *(*t_entity_creator)(t_game *, t_ftm_window *, char);
typedef struct s_weapon			t_weapon;

struct s_sprite
{
	t_list			*images;
	ssize_t			index;
	t_time			updated_at;
	t_time			update_delay;
	bool			reversed;
	bool			running;
	bool			loop;
};

struct s_item
{
	void			(*frame)(t_item *item);
	void			(*clear)(void *this);
	void			(*use)(t_item *item);
	char			identifier;
	char			*name;
	char			*description;
	bool			weapon;
	bool			food;
	bool			can_use;
	t_entity		*user;
	bool			prev_using;
	bool			already_using;
	bool			single_use;
	t_fta_audio		*cant_sound;
	t_time			last_cant_use_sound_play;
	t_time			last_use;
	t_fta_audio		*use_sound;
	t_sprite		*icon_use_sprite;
	t_sprite		*_icon_sprite;
	t_sprite		*icon_sprite;
	t_sprite		*screen_use_sprite;
	t_sprite		*screen_sprite;
	t_sprite		*_screen_sprite;
};

struct s_food
{
	t_item	item;
	int		health;
};

struct s_ammo
{
	t_item	item;
	int		amount;
};

struct s_weapon
{
	t_item		item;
	int			damage;
	double		range;
	int			ammo_usage;
};

enum e_entity_type
{
	ENTITY,
	ENTITY_PLAYER,
	ENTITY_WALL,
	ENTITY_DOOR,
	ENTITY_BILLBOARD,
	ENTITY_DROP,
	ENTITY_CHARACTER,
};

struct s_controller
{
	void		(*key)(t_entity *entity, int key, bool down);
	void		(*frame)(t_entity *entity, double delta_time);
	t_time		last_shot;
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
};

struct s_entity
{
	void			(*frame)(t_entity *entity, double delta_time);
	void			(*clear)(void *this);
	void			(*action)(t_entity *entity, t_entity *actioner);
	void			(*shot)(t_entity *shooted, t_entity *shooter);
	t_controller	controller;
	bool			targetable;
	bool			transparent;
	int				max_health;
	int				health;
	bool			hard;
	bool			wall;
	bool			actionable;
	bool			billboard;
	char			identifier;
	bool			active;
	int				ammo;
	t_time			last_inventory_scroll;
	t_item			*inventory[INVENTORY_SIZE];
	int				inventory_index;
	t_coords		coords;
	t_dsize			size;
	t_entity_type	type;
	t_entity		*target_entity;
	t_direction		target_entity_direction;
	t_fta_audio		*collision_sound;
};

struct s_camera
{
	t_entity		*entity;
	double			fov;
	unsigned int	rays;
	double			*ray_distances;
};

struct s_billboard
{
	t_entity		entity;
	t_sprite		**sprites;
};

struct s_character
{
	t_billboard billboard;
	t_sprite	**death_sprite;
	t_sprite	**hit_sprite;
	t_sprite	**walking_sprite;
	t_sprite	**_sprite;
	t_time		last_hit;
	t_fta_audio	*hit_sound;
	t_fta_audio	*death_sound;
	bool		dead;
};

struct s_player
{
	t_character character;
};

struct s_drop
{
	t_billboard	billboard;
	bool		auto_use;
	bool		auto_pickup;
};

struct s_wall
{
	t_entity	entity;
	t_sprite	*north_sprite;
	t_sprite	*south_sprite;
	t_sprite	*west_sprite;
	t_sprite	*east_sprite;
};

struct s_door
{
	t_wall		wall;
	t_direction	direction;
	t_sprite	opening_sprite;
	t_sprite	*door_sprite;
	t_sprite	*door_sides_sprite;
	bool		opened;
	bool		cant_close;
	t_time		auto_close_delay;
	t_time		last_opened_at;
	int			last_animation_index;
	t_fta_audio	*open_sound;
	t_fta_audio	*close_sound;
};

struct s_identifiers
{
	t_list	*wall;
	t_list	*player;
	t_list	*door;
	t_list	*billboard;
	t_list	*air;
	t_list	*item;
	t_list	*food;
	t_list	*drop;
	t_list	*weapon;
	t_list	*character;
	t_list	*ammo;
};

struct s_map
{
	char			*path;
	t_hashmap		*types;
	char			**raw;
	char			**map;
	t_size			size;
	t_identifiers	identifiers;
};

struct s_environment
{
	unsigned int	floor_color;
	unsigned int	ceiling_color;
};

struct s_hud_minimap
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
};

struct s_hud_debug
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
	char		*target_health;
	char		*target_id;
	char		*player_x;
	char		*player_y;
	char		*player_health;
	char		*player_yaw;
	char		*player_fov;
	char		*entities_count;
	bool		enabled;
};

struct s_hud_stats
{
	t_ftm_font	*font;
	t_sprite	*sprite;
	t_list		*states_list;
	int			states;
	int			prev_health;
	bool		enabled;
};

struct s_hud_action
{
	t_ftm_font	*font;
	bool		enabled;
};

struct s_hud
{
	t_entity		*entity;
	t_hud_debug		debug;
	t_hud_stats		stats;
	t_hud_action	action;
	t_hud_minimap	minimap;
	bool			enabled;
};

struct s_fps
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
};

struct s_game
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
	t_hashmap			*sprites_3d;
	t_hashmap			*sprites;
	t_entity			***walls;
	t_entity			**billboards;
	t_ftt_thread		*camera_threads[CAMERA_THREADS];
};

struct s_cub3d
{
	t_sprite			placeholder;
	t_map				*curr_map;
	t_ftm_window		window;
	t_game				game;
};

// cub3d
t_cub3d		*cub3d(void);
void		cub3d_exit(int code);
void		*hashmap_get_with_identifier(t_game *game, t_hashmap *hashmap, char identifier, char *rest);

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
void		key_hook(int key, t_coords coords, bool down);
void		mouse_hook(t_coords coords);

// Sprites
t_ftm_image	*get_sprite_image(t_sprite *sprite);
t_sprite	*get_entity_sprite(t_entity *entity, t_direction direction);
void		free_sprite(void *data);
void		clear_sprite(void *data);
void		init_sprite(t_sprite *sprite, t_list *images, t_time update_delay);
t_sprite	*sprite_new(t_list *images, t_time update_delay);

// Entities
t_entity_creator	get_entity_creator(t_identifiers *identifiers, char identifier);
void				free_entity(void *data);
void				call_entity_frames(t_list *entities, t_fps *fps);
void				call_entity_keys(t_list *entities, int key, bool down);
bool				entity_x_is_transparent(t_entity *entity, t_direction direction, double x);
t_player			*player_new(t_game *game, t_ftm_window *window, char identifier);
t_wall				*wall_new(t_game *game, t_ftm_window *window, char identifier);
t_door				*door_new_e(t_game *game, t_ftm_window *window, char identifier);
t_billboard			*billboard_new(t_game *game, t_ftm_window *window, char identifier);
t_entity			*entity_new(t_game *game, t_ftm_window *window, char identifier);
t_drop				*drop_new(t_game *game, t_ftm_window *window, char identifier);
t_character			*character_new(t_game *game, t_ftm_window *window, char identifier);

// Items
t_item_creator	get_item_creator(t_identifiers *identifiers, char identifier);
void			free_item(void *data);
t_item			*item_new(t_game *game, t_ftm_window *window, char identifier);
t_food			*food_new(t_game *game, t_ftm_window *window, char identifier);
t_weapon		*weapon_new(t_game *game, t_ftm_window *window, char identifier);
t_ammo			*ammo_new(t_game *game, t_ftm_window *window, char identifier);

#endif
