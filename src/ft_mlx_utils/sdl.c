/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 23:22:31 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/28 23:34:57 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_mlx_utils.h>

t_sdl_context	*sdl_context(void)
{
	static t_sdl_context	context;

	return (&context);
}

bool	update_sdl_usage(int value)
{
	if (!sdl_context()->usage_count && value > 0)
	{
		SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
			return (false);
		SDL_GameControllerEventState(SDL_ENABLE);
	}
	sdl_context()->usage_count += value;
	if (!sdl_context()->usage_count)
		(SDL_GameControllerEventState(SDL_DISABLE), SDL_Quit());
	return (true);
}
