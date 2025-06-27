/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:14:52 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 20:14:58 by afpachec         ###   ########.fr       */
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
# define W_TITLE "cub3d"
# define W_WIDTH 1024
# define W_HEIGHT 768

// Default minimap size multipliers
# define MINIMAP_WIDTH_MULTIPLIER 0.25
# define MINIMAP_HEIGHT_MULTIPLIER 0.25

// Default Font
# define DEFAULT_FONT_PATH "assets/fonts/IBM-VGA-8x16"

// Main Images
# define PLACEHOLDER_IMAGE_PATH "assets/textures/placeholder.xpm"
# define LOADING_IMAGE_PATH "assets/textures/loading.xpm"

// Player Config
# define PLAYER_RAYS_NO_HIT_LENGTH 50.0
# define PLAYER_FOV 66.0
# define PLAYER_RAY_SUBRAYS 5
# define PLAYER_MOUSE_LOOK_VELOCITY 20.0
# define PLAYER_KEY_LOOK_VELOCITY 180.0
# define PLAYER_WALK_VELOCITY 4.0
# define PLAYER_SPRINT_VELOCITY 6.0
# define PLAYER_RAY_HIT_ENTITIES_NUMBER 5
# define PLAYER_MAX_TARGET_DISTANCE 1.2
# define PLAYER_WIDTH 0.23
# define PLAYER_HEIGHT 0.23
# define PLAYER_MAX 4

// Billboard Config
# define BILLBOARD_WIDTH 0.23
# define BILLBOARD_HEIGHT 0.23

// Character Config
# define CHARACTER_HIT_DELAY 100.0
# define CHARACTER_DEFAULT_MAX_HEALTH 100

// Entity Config
# define INVENTORY_SIZE 9
# define INVENTORY_SCROLL_DELAY 0.5

// Wall Config
# define WALL_INTERACTION_DISTANCE 2.0

// Door Config
# define DOOR_ANIMATION_FPS 6

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
typedef struct s_elevator		t_elevator;
typedef struct s_identifiers	t_identifiers;
typedef struct s_item			t_item;
typedef struct s_collectible	t_collectible;
typedef struct s_drop			t_drop;
typedef void					*(*t_type_creator)(t_game *, t_ftm_window *,
									char);
typedef struct s_weapon			t_weapon;
typedef struct s_render_config	t_render_config;

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
	void			(*use)(t_item *item, t_drop *drop);
	void			(*drop)(t_game *, t_ftm_window *, t_item *,
			t_character *dropper);
	char			identifier;
	char			*name;
	char			*description;
	bool			weapon;
	bool			collectible;
	bool			can_use;
	t_character		*user;
	bool			prev_using;
	bool			already_using;
	bool			single_use;
	t_time			use_delay;
	t_fta_audio		*cant_sound;
	t_time			last_cant_use_sound_play;
	t_time			last_use;
	t_fta_audio		*use_sound;
	t_fta_audio		*get_sound;
	t_sprite		*icon_use_sprite;
	t_sprite		*_icon_sprite;
	t_sprite		*icon_sprite;
	t_sprite		*screen_use_sprite;
	t_sprite		*screen_sprite;
	t_sprite		*_screen_sprite;
};

struct s_collectible
{
	t_item	item;
	int		health;
	int		score;
	int		ammo;
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
	ENTITY_ELEVATOR
};

struct s_controller
{
	void		(*key)(t_game *game, t_entity *entity, t_ftm_key_hook_values);
	void		(*frame)(t_game *game, t_entity *entity, double delta_time);
	t_time		last_shot;
	t_time		last_strafe;
	t_time		last_seen_target;
	t_coords	last_target_position;
	t_entity	*prev_target;
	double		prev_angle;
	double		moving_to_angle;
	double		time_accumulator;
	double		optimal_proximity;
	bool		walking_forward;
	bool		walking_left;
	bool		walking_backward;
	bool		walking_right;
	bool		looking_right;
	bool		looking_left;
	bool		sprinting;
	bool		action;
	bool		keyboard_only;
	bool		already_actioned;
	double		mouse_moviment;
	double		mouse_look_velocity;
	double		key_look_velocity;
	double		walk_velocity;
	double		sprint_velocity;
};

struct s_entity
{
	void			(*frame)(t_game *game, t_entity *entity, double delta_time);
	void			(*clear)(void *this);
	void			(*action)(t_entity *entity, t_character *actioner);
	void			(*shot)(t_entity *shooted, t_character *shooter);
	t_controller	controller;
	bool			targetable;
	bool			transparent;
	bool			ultra_mega_transparent;
	bool			no_transparency_for_bill;
	int				max_health;
	int				health;
	bool			invencible;
	bool			hard;
	bool			wall;
	bool			actionable;
	bool			billboard;
	bool			character;
	char			identifier;
	bool			active;
	t_coords		coords;
	t_dsize			size;
	t_entity_type	type;
	t_fta_audio		*collision_sound;
};

struct s_render_config
{
	t_ftm_image		*canvas;
	t_game			*game;
	t_character		*character;
	double			fov;
	int				rays;
};

struct s_camera
{
	t_character		*character;
	double			fov;
	int				rays;
	double			*ray_distances;
};

struct s_billboard
{
	t_entity		entity;
	t_sprite		**sprites;
};

struct s_character
{
	t_billboard	billboard;
	t_sprite	**using_sprite;
	t_sprite	**death_sprite;
	t_sprite	**hit_sprite;
	t_sprite	**walking_sprite;
	t_sprite	**_sprite;
	t_time		last_hit;
	t_time		last_use;
	t_time		last_auto_use;
	t_fta_audio	*hit_sound;
	t_fta_audio	*death_sound;
	t_entity	*target_entity;
	t_character	*last_hit_by_character;
	t_direction	target_entity_direction;
	t_time		last_inventory_scroll;
	bool		cheating;
	t_item		*inventory[INVENTORY_SIZE];
	t_drop		*drop;
	bool		drop_items;
	double		fov;
	int			rays;
	int			inventory_index;
	char		last_used_item_identifier;
	int			ammo;
	int			score;
	bool		was_already_dead;
	bool		dead;
};

struct s_player
{
	t_character			character;
	t_ftm_image			*canvas;
	t_coords			last_canvas_pos;
	int					controller_id;
	bool				friendly_fire;
};

struct s_drop
{
	t_billboard	billboard;
	t_item		*item;
	t_item		*prev_item;
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

struct s_elevator
{
	t_wall		wall;
	char		*map_path;
};

struct s_door
{
	t_wall		wall;
	t_direction	direction;
	t_sprite	*opening_sprite;
	t_sprite	*door_sprite;
	t_sprite	*door_sides_sprite;
	bool		opened;
	bool		cant_close;
	t_time		auto_close_delay;
	t_time		last_opened_at;
	int			last_animation_index;
	bool		closeable;
	t_time		animation_delay;
	int			animation_frames;
	t_fta_audio	*open_sound;
	t_fta_audio	*close_sound;
};

struct s_map
{
	char			*path;
	t_hashmap		*types;
	char			**raw;
	char			**map;
	t_size			size;
	t_hashmap		*identifiers;
};

struct s_environment
{
	unsigned int	floor_color;
	unsigned int	ceiling_color;
};

struct s_hud_minimap
{
	t_coords		coords;
	t_size			size;
	bool			full;
	double			width_multiplier;
	double			height_multiplier;
	unsigned int	background_color;
	unsigned int	border_color;
	unsigned int	entity_color;
	unsigned int	door_color;
	unsigned int	elevator_color;
	unsigned int	wall_color;
	unsigned int	player_color;
	unsigned int	player_ray_color;
	unsigned int	player_middle_ray_color;
	double			zoom_level;
};

struct s_hud_stats
{
	t_sprite	*sprite;
	t_sprite	*cheat_sprite;
	t_sprite	*dead_sprite;
	t_list		*states_list;
	int			states;
	int			prev_health;
};

struct s_hud
{
	t_ftm_font		*action_font;
	bool			action_enabled;
	t_ftm_font		*debug_font;
	bool			debug_enabled;
	bool			minimap_enabled;
	t_ftm_font		*stats_font;
	bool			stats_enabled;
	t_hud_stats		stats;
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
	t_player			*players[PLAYER_MAX];
	int					player_controllers[PLAYER_MAX];
	t_list				*entities;
	t_hashmap			*sprites_3d;
	t_hashmap			*sprites;
	t_entity			***walls;
	t_entity			**billboards;
	t_fta_audio			*background_sound;
	t_sprite			*background_sprite;
	t_ftt_thread		*camera_threads[CAMERA_THREADS];
};

struct s_cub3d
{
	t_ftm_image			*placeholder_image;
	t_ftm_image			*loading_image;
	char				*new_map_path;
	t_map				*curr_map;
	t_ftm_window		*window;
	pthread_mutex_t		game_mutex;
	t_game				*game;
};

// cub3d
t_cub3d				*cub3d(void);
void				cub3d_exit(int code);
void				*hashmap_get_with_identifier(t_game *game,
						t_hashmap *hashmap, char identifier, char *rest);
void				update_walls_matrix(t_game *game);
void				update_billboards_vec(t_game *game);
t_type_creator		get_type_creator(t_hashmap *identifiers, char identifier);
void				free_walls(t_game *game);

// Game
t_game				*game_new_e(t_ftm_window *window, t_map *map);
void				clear_game(void *game);
void				free_game(void *game);
void				render_game(t_game *game, t_ftm_image *canvas,
						t_character *character);
void				render_players_game(t_game *game, t_ftm_window *window);

// Map
t_map				*parse_map_e(char *path);
void				clear_map(void *map);
void				free_map(t_map *map);

// Loop
void				loop(void);
void				key_hook(t_ftm_key_hook_values key_hook_values);
void				mouse_hook(t_coords coords);

// Sprites
t_ftm_image			*get_sprite_image(t_sprite *sprite);
t_ftm_image			*get_sprite3d_image(t_sprite **sprite3d, double angle);
t_sprite			*get_entity_sprite(t_entity *entity, t_direction direction);
void				free_sprite(void *data);
void				clear_sprite(void *data);
void				init_sprite(t_sprite *sprite, t_list *images,
						t_time update_delay);
t_sprite			*sprite_new(t_list *images, t_time update_delay);
void				sprite_soft_copy(t_sprite **dst, t_sprite *src);

// Entities
void				free_entity(void *data);
void				call_entity_frames(t_game *game, t_fps *fps);
void				call_entity_keys(t_game *game, t_ftm_key_hook_values kvh);
bool				entity_x_is_transparent(t_entity *entity,
						t_direction direction, double x);
t_player			*player_new(t_game *game, t_ftm_window *window,
						char identifier);
t_wall				*wall_new(t_game *game, t_ftm_window *window,
						char identifier);
t_door				*door_new_e(t_game *game, t_ftm_window *window,
						char identifier);
t_billboard			*billboard_new(t_game *game, t_ftm_window *window,
						char identifier);
t_entity			*entity_new(t_game *game, t_ftm_window *window,
						char identifier);
t_drop				*drop_new(t_game *game, t_ftm_window *window,
						char identifier);
t_character			*character_new(t_game *game, t_ftm_window *window,
						char identifier);
t_elevator			*elevator_new(t_game *game, t_ftm_window *window,
						char identifier);

// Items
void				free_item(void *data);
t_item				*item_new(t_game *game, t_ftm_window *window,
						char identifier);
t_collectible		*collectible_new(t_game *game, t_ftm_window *window,
						char identifier);
t_weapon			*weapon_new(t_game *game, t_ftm_window *window,
						char identifier);

#endif
