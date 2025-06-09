/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/09 17:05:47 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "character.h"

void	character_frame(t_entity *entity, double delta_time)
{
	t_character *character;

	billboard_frame(entity, delta_time);
	character = (t_character *)entity;
	if (character->dead)
	{
		character->billboard.entity.controller.frame = NULL;
		character->billboard.sprites = character->death_sprite;
	}
	else if (ft_get_time() - character->last_hit < CHARACTER_HIT_DELAY)
		character->billboard.sprites = character->hit_sprite;
	else if (entity->controller.walking_right
		|| entity->controller.walking_backward
		|| entity->controller.walking_forward
		|| entity->controller.walking_left)
		character->billboard.sprites = character->walking_sprite;
	else
		character->billboard.sprites = character->_sprite;
}

void	clear_character(void *character)
{
	clear_billboard(character);
}

void	character_action(t_entity *entity, t_entity *actioner)
{
	billboard_action(entity, actioner);
}

void	character_shot(t_entity *shooted, t_entity *shooter)
{
	t_character	*character;
	int			i;

	billboard_shot(shooted, shooter);
	character = (t_character *)shooted;
	if (!character->dead)
	{
		if (!shooted->health)
		{
			fta_play(character->death_sound);
			i = -1;
			while (++i < 360)
			{
				if (character->death_sprite[i])
				{
					character->death_sprite[i]->index = 0;
					character->death_sprite[i]->running = true;
				}
			}
			character->dead = true;
		}
		else
			fta_play(character->hit_sound);
	}
	character->last_hit = ft_get_time();
}
