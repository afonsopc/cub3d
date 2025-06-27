/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controllers2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:22:31 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:39:37 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controllers.h"

static double	normalize_pressure(int16_t value)
{
	return (fmax(0.0, (double)value / 32767.0));
}

static void	set_to_raw_if_x(bool is_x, int16_t *x, int16_t *y, int16_t raw)
{
	if (is_x)
		*x = raw;
	else
		*y = raw;
	if (abs(*x) < CONTROLLERS_DEADZONE)
		*x = 0;
	if (abs(*y) < CONTROLLERS_DEADZONE)
		*y = 0;
}

static t_ftm_key_hook_values	get_joystick_vals(t_ftm_controller *controller,
	int axis, int16_t raw)
{
	t_ftm_key_hook_values	val;
	bool					is_left;
	bool					is_x;

	val = (t_ftm_key_hook_values){0};
	is_x = axis == SDL_CONTROLLER_AXIS_LEFTX
		|| axis == SDL_CONTROLLER_AXIS_RIGHTX;
	is_left = axis == SDL_CONTROLLER_AXIS_LEFTX
		|| axis == SDL_CONTROLLER_AXIS_LEFTY;
	if (is_left)
		set_to_raw_if_x(is_x, &controller->lx, &controller->ly, raw);
	else
		set_to_raw_if_x(is_x, &controller->rx, &controller->ry, raw);
	val.key = FTM_GAMEPAD_RSTICK;
	val.coords.x = fabs((double)controller->rx / 32767.0 * 0.5 + 0.5);
	val.coords.y = fabs((double)controller->ry / 32767.0 * 0.5 + 0.5);
	if (is_left)
	{
		val.key = FTM_GAMEPAD_LSTICK;
		val.coords.x = fabs((double)controller->lx / 32767.0 * 0.5 + 0.5);
		val.coords.y = fabs((double)controller->ly / 32767.0 * 0.5 + 0.5);
	}
	return (val);
}

static t_ftm_key_hook_values	get_axis_vals(t_ftm_controller *controller,
	SDL_Event *event)
{
	t_ftm_key_hook_values	val;
	int						axis;
	int16_t					raw;

	val = (t_ftm_key_hook_values){0};
	raw = event->caxis.value;
	axis = event->caxis.axis;
	if (axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT
		|| axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
	{
		val.key = ft_ternary_int(axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT,
				FTM_GAMEPAD_L2, FTM_GAMEPAD_R2);
		val.pressure = normalize_pressure(raw);
		val.down = (val.pressure > 0.05);
		return (val);
	}
	return (get_joystick_vals(controller, axis, raw));
}

void	ftm_controller_event_handler(void *userdata, SDL_Event *event)
{
	t_ftm_controller		*controller;
	t_ftm_key_hook_values	val;

	controller = (t_ftm_controller *)userdata;
	if (!controller || !controller->key_hook)
		return ;
	val = (t_ftm_key_hook_values){0};
	if (event->type == SDL_CONTROLLERBUTTONDOWN
		|| event->type == SDL_CONTROLLERBUTTONUP)
	{
		if (event->cbutton.which != controller->id)
			return ;
		val.key = -(event->cbutton.button + 1);
		val.down = (event->type == SDL_CONTROLLERBUTTONDOWN);
	}
	else if (event->type == SDL_CONTROLLERAXISMOTION)
	{
		if (event->caxis.which != controller->joy_id)
			return ;
		val = get_axis_vals(controller, event);
	}
	else
		return ;
	val.controller = controller;
	controller->key_hook(val);
}
