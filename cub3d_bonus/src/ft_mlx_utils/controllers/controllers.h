/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:46:32 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:35:59 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTROLLERS_H
# define CONTROLLERS_H

# include <ft_mlx_utils.h>

# define CONTROLLERS_DEADZONE 8000

typedef struct s_sdl_context
{
	int		usage_count;
}	t_sdl_context;

t_sdl_context	*sdl_context(void);
void			update_sdl_usage_e(int value);

#endif
