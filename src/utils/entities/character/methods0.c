/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/12 19:55:15 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "character.h"

static void	set_using(t_game *game, t_character *character)
{
	char		*key;
	t_sprite	**original;

	key = ft_strf("USING_%c",
		character->inventory[character->inventory_index]->identifier);
	original = hashmap_get_with_identifier(game, game->sprites_3d,
		character->billboard.entity.identifier, key);
	free(key);
	if (!original)
		return ;
	if (character->billboard.sprites != character->using_sprite
		|| character->last_used_item_identifier != character->inventory[character->inventory_index]->identifier)
		fill_3d_sprites_from_src(character->using_sprite, original);
	character->billboard.sprites = character->using_sprite;
	character->last_used_item_identifier = character->inventory[character->inventory_index]->identifier;
}

static void	set_dead(t_character *character)
{
	character->billboard.entity.controller.frame = NULL;
	character->billboard.sprites = character->death_sprite;
}

static bool	walking(t_entity *entity)
{
	return (entity->controller.walking_right
		|| entity->controller.walking_backward
		|| entity->controller.walking_forward
		|| entity->controller.walking_left);
}

static bool	hit(t_character *character)
{
	return (ft_get_time() - character->last_hit < CHARACTER_HIT_DELAY);
}

static bool	using(t_character *character)
{
	return (character->inventory[character->inventory_index]
		&& (ft_get_time() - character->inventory[character->inventory_index]->last_use < character->inventory[character->inventory_index]->screen_sprite->update_delay
			|| character->inventory[character->inventory_index]->user == character));
}

static void	call_item_frames(t_character *character)
{
	int	i;

	i = -1;
	while (++i < INVENTORY_SIZE)
		if (character->inventory[i] && character->inventory[i]->frame)
			character->inventory[i]->frame(character->inventory[i]);
}

void	character_frame(t_entity *entity, double delta_time)
{
	t_character *character;

	billboard_frame(entity, delta_time);
	character = (t_character *)entity;
	if (character->dead)
		set_dead(character);
	else if (hit(character))
		character->billboard.sprites = character->hit_sprite;
	else if (using(character))
	set_using(&cub3d()->game, character);
	else if (walking(entity))
		character->billboard.sprites = character->walking_sprite;
	else
		character->billboard.sprites = character->_sprite;
	call_item_frames(character);
}

void	clear_character(void *character)
{
	clear_billboard(character);
}

void	character_action(t_entity *entity, t_character *actioner)
{
	billboard_action(entity, actioner);
}

void	character_shot(t_entity *shooted, t_character *shooter)
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
					character->death_sprite[i]->updated_at = ft_get_time();
				}
			}
			character->dead = true;
			shooted->hard = false;
			shooted->targetable = false;
		}
		else
			fta_play(character->hit_sound);
	}
	character->last_hit = ft_get_time();
}
