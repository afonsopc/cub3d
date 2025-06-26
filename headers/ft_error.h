/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:05:32 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 13:59:30 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ERROR_H
# define FT_ERROR_H

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>

typedef enum e_error
{
	ERROR_NO_ERROR,
	ERROR_INIT_MLX,
	ERROR_INIT_WINDOW,
	ERROR_INVALID_ARGS,
	ERROR_INVALID_MAP,
	ERROR_MAP_OPEN,
	ERROR_MAP_ALREADY_INITIALIZED,
	ERROR_MAP_READ,
	ERROR_DUPLICATE_TYPE,
	ERROR_MAP_HASHMAP,
	ERROR_ENTITY_CREATION,
	ERROR_INIT_CANVAS,
	ERROR_INVALID_TYPE,
	ERROR_LOAD_SPRITE,
	ERROR_INIT_MAP_GRID_TRIPLE,
	ERROR_INIT_MAP_GRID_DOUBLE,
	ERROR_INIT_SPRITES,
	ERROR_LOAD_PLACEHOLDER_SPRITE,
	ERROR_MAP_ALLOC,
	ERROR_EMPTY_TYPE_IDENTIFIER,
	ERROR_DUPLICATE_TYPE_IDENTIFIER,
	ERROR_INVALID_IDENTIFIER_TYPE,
	ERROR_CEILING_OR_FLOORING_UNDEFINED,
	ERROR_INVALID_FILETYPE
}	t_error;

typedef struct s_error_storage
{
	t_error	error;
	void	(*exit)(int);
}	t_error_storage;

# define ERROR_NO_ERROR_MSG "No Error"
# define ERROR_INIT_MLX_MSG "Initializing mlx"
# define ERROR_INIT_WINDOW_MSG "Initializing window"
# define ERROR_INVALID_ARGS_MSG "Invalid Arguments"
# define ERROR_INVALID_MAP_MSG "Invalid Map"
# define ERROR_MAP_OPEN_MSG "Map open error"
# define ERROR_MAP_ALREADY_INITIALIZED_MSG "Map already initialized"
# define ERROR_MAP_READ_MSG "Map read error"
# define ERROR_DUPLICATE_TYPE_MSG "Duplicate Type"
# define ERROR_MAP_HASHMAP_MSG "Map Hashmap error"
# define ERROR_INVALID_TYPE_MSG "Invalid Type"
# define ERROR_ENTITY_CREATION_MSG "Entity Creation error"
# define ERROR_INIT_CANVAS_MSG "Initializing canvas"
# define ERROR_LOAD_SPRITE_MSG "Loading sprite error"
# define ERROR_INIT_MAP_GRID_TRIPLE_MSG "Initializing map grid triple"
# define ERROR_INIT_MAP_GRID_DOUBLE_MSG "Initializing map grid double"
# define ERROR_INIT_SPRITES_MSG "Initializing sprites error"
# define ERROR_LOAD_PLACEHOLDER_SPRITE_MSG "Loading placeholder sprite error"
# define ERROR_MAP_ALLOC_MSG "Map allocation error"
# define ERROR_CEILING_OR_FLOORING_UNDEFINED_MSG "Ceiling/Flooring undefined"
# define ERROR_EMPTY_TYPE_IDENTIFIER_MSG "Empty type identifier"
# define ERROR_DUPLICATE_TYPE_IDENTIFIER_MSG "Duplicate type identifier"
# define ERROR_INVALID_IDENTIFIER_TYPE_MSG "Invalid identifier type"
# define ERROR_INVALID_MSG "Invalid Error"
# define ERROR_INVALID_FILETYPE_MSG "Invalid File Extension"

// Set ft_error global error
void			fte_set(t_error _error);

// Check if there is an error
bool			fte_flagged(void);

// Get the current error as a string
char			*fte_error_to_message(t_error _error);

// ft_error's singleton for storage
t_error_storage	*fte_storage(void);

// Assert that there are no errors.
// If an error is found, it will be printed to stdout
// and the program will exit using the `exit`
// function set in `fte_storage()->exit`
void			fte_assert(void);

#endif
