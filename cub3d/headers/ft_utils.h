/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 12:15:46 by paude-so          #+#    #+#             */
/*   Updated: 2025/05/10 21:55:42 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_UTILS_H
# define FT_UTILS_H

# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define GNL_BUFFER_SIZE 1024

typedef enum s_direction
{
	NORTH,
	SOUTH,
	WEST,
	EAST
}						t_direction;

typedef struct s_coords
{
	double				x;
	double				y;
	double				z;
	double				yaw;
}						t_coords;

typedef struct s_rgba
{
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	unsigned char		a;
}						t_rgba;

typedef struct s_size
{
	int					width;
	int					height;
}						t_size;

typedef size_t			t_time;

typedef struct s_element
{
	char				*key;
	void				*value;
	struct s_element	*next;
	void				(*value_free)(void *value);
}						t_element;

typedef struct s_hashmap
{
	t_element			**table;
}						t_hashmap;

typedef struct s_list
{
	void				*data;
	struct s_list		*next;
	void				(*data_free)(void *data);
}						t_list;

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

char					**ft_strvndup(char **v, size_t n);
char					**ft_strvdup(char **v);
void					ft_strvfree(char **v);
void					ft_bzero(void *s, size_t n);

bool					ft_strequal(char *s1, char *s2);

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
bool					ft_str_endswith(char *str, char *ending);
char					*ft_strtrim(char *s);
t_time					ft_get_time(void);
void					ft_fprint_list(int fd, t_list *list,
							char *(*to_str)(void *data));

bool					ft_list_any(t_list *list, bool (*condition)(void *data,
								void *params), void *params);
bool					ft_str_all_params(char *str, bool (*func)(char, void *),
							void *params);
char					*ft_strf(char *format, ...);

#endif