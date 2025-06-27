/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 20:46:22 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:35:20 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controllers.h"

t_sdl_context	*sdl_context(void)
{
	static t_sdl_context	context;

	return (&context);
}

void	ftm_controller_init_e(t_ftm_controller *controller, int id)
{
	fte_set(NULL);
	if (!controller)
		return ;
	update_sdl_usage_e(1);
	if (fte_flagged())
		return ;
	controller->controller = SDL_GameControllerOpen(id);
	if (!controller->controller)
		return (update_sdl_usage_e(-1), fte_set("Controller fail %d", id));
	controller->id = SDL_JoystickInstanceID(
			SDL_GameControllerGetJoystick(controller->controller));
	controller->joy_id = id;
}

void	ftm_controller_clear(void *data)
{
	t_ftm_controller	*controller;

	controller = data;
	if (!controller)
		return ;
	SDL_GameControllerClose(controller->controller);
	(update_sdl_usage_e(-1), fte_set(NULL));
	controller->controller = NULL;
	controller->id = -1;
}

t_list	*ftm_load_controllers(void)
{
	t_list				*list;
	t_ftm_controller	*controller;
	int					i;

	list = NULL;
	i = 0;
	while (true)
	{
		controller = ftm_controller_new(i);
		if (!controller)
			break ;
		printf("Controller %d: %s\n", i,
			SDL_GameControllerName(controller->controller));
		ft_list_add(&list, controller, ftm_controller_free);
		++i;
	}
	return (list);
}

void	ftm_call_controllers_event_handlers(t_list *controllers)
{
	SDL_Event	event;

	while (SDL_PollEvent(&event))
		ft_list_foreach(controllers, (void *)ftm_controller_event_handler,
			&event);
}
