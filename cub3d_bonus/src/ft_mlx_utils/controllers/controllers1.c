/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:48:27 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/27 21:18:45 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controllers.h"

t_ftm_controller	*ftm_controller_new(int id)
{
	t_ftm_controller	*new;

	new = ft_calloc(1, sizeof(t_ftm_controller));
	if (!new)
		return (NULL);
	ftm_controller_init_e(new, id);
	if (fte_flagged())
		return (fte_set(NULL), free(new), NULL);
	return (new);
}

void	ftm_controller_free(void *data)
{
	ftm_controller_clear(data);
	free(data);
}

void	update_sdl_usage_e(int value)
{
	if (!sdl_context()->usage_count && value > 0)
	{
		SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
		if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
			return (fte_set("Failed to initialize SDL: %s", SDL_GetError()));
		SDL_GameControllerEventState(SDL_ENABLE);
	}
	sdl_context()->usage_count += value;
	if (!sdl_context()->usage_count)
		(SDL_GameControllerEventState(SDL_DISABLE), SDL_Quit());
}
