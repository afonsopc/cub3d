/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 12:15:46 by paude-so          #+#    #+#             */
/*   Updated: 2025/06/25 21:25:23 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_UTILS_H
# define FT_UTILS_H

# include <errno.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>
# include <math.h>
# include <fcntl.h>
# include <stdarg.h>
# include <dirent.h>

# define FT_UTILS_PI 3.14159265359
# define FT_GNL_BUFF 1024
# define FT_MAX_RAY_LENGTH 150.0
# define FT_EPSILON 1e-9

typedef enum s_direction
{
	NORTH,
	SOUTH,
	WEST,
	EAST
}	t_direction;

typedef struct s_coords
{
	double	x;
	double	y;
	double	yaw;
}	t_coords;

typedef struct s_ten_ints
{
	int	one;
	int	two;
	int	three;
	int	four;
	int	five;
	int	six;
	int	seven;
	int	eight;
	int	nine;
	int	ten;
}	t_ten_ints;

typedef struct s_rgba
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}	t_rgba;

typedef struct s_size
{
	int	width;
	int	height;
}	t_size;

typedef struct s_dsize
{
	double	width;
	double	height;
}	t_dsize;

typedef size_t	t_time;

typedef struct s_element
{
	char				*key;
	void				*value;
	struct s_element	*next;
	void				(*value_free)(void *value);
}						t_element;

typedef struct s_hashmap
{
	t_element			*table;
}						t_hashmap;

typedef struct s_list
{
	void				*data;
	struct s_list		*next;
	void				(*data_free)(void *data);
}						t_list;

typedef struct s_dda_raycast_data
{
	double			length;
	t_coords		rdir;
	t_coords		deltadist;
	t_coords		side_dist;
	t_coords		pos;
	t_coords		step;
	int				side;
	double			hit_x;
	void			*hit;
	void			***objs;
	t_size			objs_size;
	t_direction		hitdir;
	t_coords		sp;
	void			*ignored_obj;
}	t_dda_raycast_data;

typedef struct s_dda_raycast_config
{
	void		***objs;
	t_size		objs_size;
	t_coords	start_pos;
	void		*ignored_obj;
}	t_dda_raycast_config;

typedef struct s_raycast
{
	double		distance;
	double		yaw;
	void		*hit;
	double		hit_x;
	t_direction	hit_direction;
	t_coords	hit_coords;
}	t_raycast;

double					ft_normalize_angle(double angle);
int						ft_atoi(const char *nptr);
t_list					*ft_list_index(t_list *list, size_t index);
bool					ft_is_file(char *path);
unsigned int			ft_rgb_to_unsigned(char *rgb_str, char *separator);

size_t					ft_strlen(const char *s);
size_t					ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t					ft_strlcat(char *dst, const char *src, size_t dstsize);
char					*ft_strjoin(char const *s1, char const *s2);
ssize_t					ft_fputstr(int fd, char *string);

char					*ft_strdup(const char *s1);
char					*ft_strndup(const char *s1, size_t n);
bool					ft_isalpha(int c);
char					*ft_strnstr(const char *haystack, const char *needle,
							size_t len);
char					*ft_strstr(const char *haystack, const char *needle);

int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_strcmp(const char *s1, const char *s2);
bool					ft_isspace(char c);

bool					ft_isalnum(int c);
char					*ft_strchr(const char *s, int c);
void					*ft_calloc(size_t count, size_t size);
size_t					ft_strvlen(char **v);
char					**ft_strvjoin(char **v1, char **v2);

char					**ft_split(const char *s, const char *c);
void					*ft_memcpy(void *dst, const void *src, size_t n);

char					*ft_charjoin(char *str, char c);
char					*ft_itoa(int n);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
bool					ft_strequal(const char *s1, const char *s2);

char					**ft_strvndup(char **v, size_t n);
char					**ft_strvdup(char **v);
void					ft_strvfree(char **v);
void					ft_bzero(void *s, size_t n);

t_list					*ft_list_new(void *data, void (*data_free)(void *data));
void					ft_list_add(t_list **list, void *data,
							void (*data_free)(void *data));
void					ft_list_destroy(t_list **list);
size_t					ft_list_size(t_list *list);
char					**ft_list_to_strv(t_list *list,
							char *(*to_str)(void *data));

t_hashmap				*ft_hashmap_new(void);
t_element				*ft_element_new(char *key, void *value,
							void (*el_free)(void *value));
t_element				*ft_hashmap_get(t_hashmap *hashmap, char *key);
void					*ft_hashmap_get_value(t_hashmap *hashmap, char *key);
void					ft_hashmap_set(t_hashmap *hashmap, char *key,
							void *value, void (*el_free)(void *value));
void					ft_hashmap_destroy(t_hashmap *hashmap);
char					**ft_hashmap_to_strv(t_hashmap *hashmap);
void					ft_fprint_hashmap(int fd, t_hashmap *hashmap);
void					ft_hashmap_del(t_hashmap *hashmap, char *key);

void					ft_close(int fd);
void					ft_close2(int fd1, int fd2);
void					ft_fprint_strv(int fd, char **strv);
bool					ft_isdigit(int c);
char					*ft_strappend(char *str, char append);

char					*ft_get_next_line(int fd);

bool					ft_str_all(char *str, bool (*func)(char));
bool					ft_str_endswith(const char *str, const char *ending);
char					*ft_strtrim(char *s);
t_time					ft_get_time(void);
void					ft_fprint_list(int fd, t_list *list,
							char *(*to_str)(void *data));

bool					ft_list_any(t_list *list, bool (*condition)(void *data,
								void *params), void *params);
bool					ft_str_all_params(char *str, bool (*func)(char, void *),
							void *params);
char					*ft_strfva(const char *format, va_list *args);
double					ft_atof(const char *str);

t_list					*ft_list_clone(t_list *list);
void					ft_list_reverse(t_list **list);
void					ft_sleep(double milliseconds);
void					ft_list_clear_without_free(t_list *list);
void					ft_list_free(t_list *list);

double					ft_cos_degrees(double angle_degree);
double					ft_sin_degrees(double angle_degree);
double					ft_distance(t_coords a, t_coords b);
double					ft_radians(double angle_degree);
double					ft_degrees(double angle_radians);

double					ft_angle_distance(t_coords a, t_coords b);
bool					ft_is_directory(const char *path);
char					*ft_clean_path(const char *path);
void					ft_strvorder(char **strv, const void *data,
							bool (*cmp)(const void *data, const char *,
								const char *));
t_direction				ft_direction_from_str(const char *str);

bool					ft_str_equal_char_ptr(const char *str,
							const char *char_ptr);
char					*ft_strf(const char *format, ...);
char					*ft_strnrstr(const char *haystack, const char *needle,
							size_t len);
char					*ft_strrstr(const char *haystack, const char *needle);
char					*ft_strrchr(const char *s, int c);

t_raycast				ft_dda_raycast(t_dda_raycast_config ddarc);

double					ft_fraction(double value);
void					*ft_ternary_ptr(void *con, void *_if, void *_else);
double					ft_ternary_double(bool con, double _if, double _else);
int						ft_ternary_int(bool con, int _if, int _else);
void					ft_list_foreach(t_list *list,
							void (*f)(void *, void *param), void *param);

bool					ft_list_all(t_list *list,
							bool (*condition)(void *data, void *params),
							void *params);
size_t					ft_list_count(t_list *list,
							bool (*condition)(void *data, void *params),
							void *params);

#endif
