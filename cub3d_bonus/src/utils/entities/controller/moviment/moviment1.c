/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moviment1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:36:12 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:38:33 by afpachec         ###   ########.fr       */
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
		return (false);
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

void	move_player_x(t_list *entities, t_entity *entity, double angle,
	double velocity)
{
	double	new_x;

	new_x = entity->coords.x + velocity * ft_cos_degrees(angle);
	if (!position_overlaps(entities, entity, (t_coords){new_x, entity->coords.y,
			0}))
		entity->coords.x = new_x;
}

void	move_player_y(t_list *entities, t_entity *entity, double angle,
	double velocity)
{
	double	new_y;

	new_y = entity->coords.y + velocity * ft_sin_degrees(angle);
	if (!position_overlaps(entities, entity, (t_coords){entity->coords.x,
			new_y, 0}))
		entity->coords.y = new_y;
}
