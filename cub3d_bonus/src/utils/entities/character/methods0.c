/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:31:48 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 22:45:46 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "character.h"

static void	billions_must_die(t_game *game, t_character *character)
{
	bool	was_already_dead;

	if (character->dead && !character->was_already_dead)
		drop_items(game, character);
	if (character->dead && character->billboard.entity.health)
		character->dead = false;
	was_already_dead = character->was_already_dead;
	character->was_already_dead = character->dead;
	if ((!character->dead && !was_already_dead)
		|| (character->dead && was_already_dead))
		return ;
	character->billboard.entity.hard = !character->dead;
	character->billboard.entity.targetable = !character->dead;
}

void	character_frame(t_game *game, t_entity *entity, double delta_time)
{
	t_character	*character;

	billboard_frame(game, entity, delta_time);
	character = (t_character *)entity;
	billions_must_die(game, character);
	if (character->dead)
		character->billboard.sprites = character->death_sprite;
	else if (ft_get_time() - character->last_hit < CHARACTER_HIT_DELAY)
		character->billboard.sprites = character->hit_sprite;
	else if (using(character))
		set_using(game, character);
	else if (walking(entity))
		character->billboard.sprites = character->walking_sprite;
	else
		character->billboard.sprites = character->_sprite;
	call_item_frames(character);
}

void	clear_character(void *data)
{
	t_character	*character;

	if (data)
		((t_billboard *)data)->sprites = ((t_character *)data)->_sprite;
	clear_billboard(data);
	if (!data)
		return ;
	character = (t_character *)data;
	free_entity(character->drop);
	free_inventory_items(character);
	free_3d_sprite(character->hit_sprite);
	free_3d_sprite(character->death_sprite);
	free_3d_sprite(character->using_sprite);
	free_3d_sprite(character->walking_sprite);
}

void	character_action(t_entity *entity, t_character *actioner)
{
	billboard_action(entity, actioner);
}

void	character_shot(t_entity *shooted, t_character *shooter)
{
	t_character	*character;

	billboard_shot(shooted, shooter);
	character = (t_character *)shooted;
	character->last_hit = ft_get_time();
	if (character->dead)
		return ;
	character->last_hit_by_character = shooter;
	if (shooted->health)
		return (fta_play(character->hit_sound));
	fta_play(character->death_sound);
	reset_3d_sprites(character->death_sprite);
	character->dead = true;
}
