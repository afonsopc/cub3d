/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moviment1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:36:12 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/01 23:12:15 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "moviment.h"

void	looks(t_entity *entity, double delta_time)
{
	double	velocity;

	velocity = entity->controller.key_look_velocity * delta_time;
	if (entity->controller.looking_left)
		entity->coords.yaw = ft_normalize_angle(entity->coords.yaw - velocity);
	else if (entity->controller.looking_right)
		entity->coords.yaw = ft_normalize_angle(entity->coords.yaw + velocity);
}

t_entity	*position_overlaps(t_list *entities, t_entity *entity,
	t_coords coords)
{
	t_list		*curr;
	t_entity	*curr_entity;

	if (!entity->hard)
		return (NULL);
	curr = entities;
	while (curr)
	{
		curr_entity = curr->data;
		if (curr_entity != entity && curr_entity->hard && curr_entity->active
			&& coords.x + entity->size.width >= curr_entity->coords.x
			&& coords.x - entity->size.width
			<= (curr_entity->coords.x + curr_entity->size.width)
			&& coords.y + entity->size.height >= curr_entity->coords.y
			&& coords.y - entity->size.height
			<= (curr_entity->coords.y + curr_entity->size.height))
			return (fta_play(entity->collision_sound), curr_entity);
		curr = curr->next;
	}
	return (NULL);
}

static bool	boxes_touch(t_entity *a, t_coords at, t_entity *b)
{
	return (at.x + a->size.width >= b->coords.x
		&& at.x - a->size.width <= b->coords.x + b->size.width
		&& at.y + a->size.height >= b->coords.y
		&& at.y - a->size.height <= b->coords.y + b->size.height);
}

static double	center_dist(t_coords at, t_entity *b)
{
	t_coords	c;

	c = b->coords;
	if (!b->character)
	{
		c.x += b->size.width / 2.0;
		c.y += b->size.height / 2.0;
	}
	return (ft_distance(at, c));
}

/*
** A move is blocked by any hard entity it would touch, except when the
** mover already touches it and the move takes it further away: so two
** guards bumping in a doorway, or a guard pushed against a wall, can always
** move apart instead of locking up for good.
*/
static bool	can_move_to(t_list *entities, t_entity *entity, t_coords to)
{
	t_list		*curr;
	t_entity	*o;

	if (!entity->hard)
		return (true);
	curr = entities;
	while (curr)
	{
		o = curr->data;
		curr = curr->next;
		if (o == entity || !o->hard || !o->active || !boxes_touch(entity, to, o))
			continue ;
		if (boxes_touch(entity, entity->coords, o)
			&& center_dist(to, o) > center_dist(entity->coords, o))
			continue ;
		if (entity->type == ENTITY_PLAYER)
			fta_play(entity->collision_sound);
		return (false);
	}
	return (true);
}

void	move_player_x(t_list *entities, t_entity *entity, double angle,
	double velocity)
{
	double	new_x;

	new_x = entity->coords.x + velocity * ft_cos_degrees(angle);
	if (can_move_to(entities, entity, (t_coords){new_x, entity->coords.y, 0}))
		entity->coords.x = new_x;
}

void	move_player_y(t_list *entities, t_entity *entity, double angle,
	double velocity)
{
	double	new_y;

	new_y = entity->coords.y + velocity * ft_sin_degrees(angle);
	if (can_move_to(entities, entity, (t_coords){entity->coords.x, new_y, 0}))
		entity->coords.y = new_y;
}
