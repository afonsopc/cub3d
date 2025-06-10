/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:35:54 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/10 18:47:32 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "controller.h"

static void	frame(t_entity *entity, double delta_time)
{
	moviment_frame(entity, delta_time);
}

static void	mouse_inv_keys(t_entity *entity, int key)
{
	int	new_index;

	new_index = entity->inventory_index;
	if (key == XK_MOUSE_SCROLL_UP
		&& ft_get_time() - entity->last_inventory_scroll >= INVENTORY_SCROLL_DELAY)
	{
		new_index = (new_index + 1) % INVENTORY_SIZE;
			entity->last_inventory_scroll = ft_get_time();
	}
	if (key == XK_MOUSE_SCROLL_DOWN
		&& ft_get_time() - entity->last_inventory_scroll >= INVENTORY_SCROLL_DELAY)
	{
		new_index = (new_index - 1 + INVENTORY_SIZE) % INVENTORY_SIZE;
		entity->last_inventory_scroll = ft_get_time();
	}
	if (!entity->inventory[new_index])
		return ;
	entity->inventory_index = new_index;
}

static void	inv_key(t_entity *entity, int key, bool down)
{
	int	i;

	i = -1;
	if (!down)
		return ;
	mouse_inv_keys(entity, key);
	while (++i < INVENTORY_SIZE)
	{
		if (key == XK_1 + i && entity->inventory[i])
			entity->inventory_index = i;
	}
}

static void	item_key(t_entity *entity, int key, bool down)
{
	t_item	*item;

	item = entity->inventory[entity->inventory_index];
	if ((key == XK_space || key == XK_MOUSE_LEFT) && item)
	{
		if (down)
			item->user = entity;
		else
			item->user = NULL;
	}
}

static void	key(t_entity *entity, int key, bool down)
{
	if (key == XK_w)
		entity->controller.walking_forward = down;
	if (key == XK_a)
		entity->controller.walking_left = down;
	if (key == XK_s)
		entity->controller.walking_backward = down;
	if (key == XK_d)
		entity->controller.walking_right = down;
	if (key == XK_Right)
		entity->controller.looking_right = down;
	if (key == XK_Left)
		entity->controller.looking_left = down;
	if (key == XK_e)
		entity->controller.action = down;
	if (key == XK_Shift_L || key == XK_Shift_R)
		entity->controller.sprinting = down;
	item_key(entity, key, down);
	inv_key(entity, key, down);
}

void	init_player_controller(t_entity *entity)
{
	entity->controller.frame = frame;
	entity->controller.key_look_velocity = PLAYER_KEY_LOOK_VELOCITY;
	entity->controller.mouse_look_velocity = PLAYER_MOUSE_LOOK_VELOCITY;
	entity->controller.walk_velocity = PLAYER_WALK_VELOCITY;
	entity->controller.sprint_velocity = PLAYER_SPRINT_VELOCITY;
	entity->controller.key = key;
}
