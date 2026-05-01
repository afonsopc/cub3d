/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 23:22:31 by afpachec          #+#    #+#             */
/*   Updated: 2025/08/13 12:02:19 by afpachec         ###   ########.fr       */
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
		SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		SDL_SetHint(SDL_HINT_VIDEO_MAC_FULLSCREEN_SPACES, "0");
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
			return (false);
		SDL_GameControllerEventState(SDL_ENABLE);
	}
	sdl_context()->usage_count += value;
	if (!sdl_context()->usage_count)
		(SDL_GameControllerEventState(SDL_DISABLE), SDL_Quit());
	return (true);
}
