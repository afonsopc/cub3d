/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   story4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 18:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 18:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Puzzles: chaves, documentos e alavancas. Tudo o que o jogador "tem" é um
** carácter no chaveiro (game->keyring): uma chave apanhada (COLLECTIBLE com
** GIVES c), uma alavanca puxada (ELEVATOR com GIVES c). Portas com KEY c só
** abrem com c; portões com KEY_OPENS TRUE abrem-se sozinhos quando chega c.
** Os objetivos (OBJECTIVE_n_TEXT / OBJECTIVE_n_DONE c) avançam pelo chaveiro
** e aparecem no canto do ecrã; o rádio pode disparar com RADIO_n_GOT c.
*/

#include "story.h"

bool	game_has_key(t_game *game, char key)
{
	return (game && key && ft_strchr(game->keyring.keys, key));
}

void	game_toast(t_game *game, const char *latin1)
{
	if (!game || !latin1)
		return ;
	ft_strlcpy(game->keyring.toast, latin1, sizeof(game->keyring.toast));
	game->keyring.toast_at = ft_get_time();
	printf("STORY toast %s\n", game->keyring.toast);
}

static void	open_gates(t_game *game, char key)
{
	t_list	*l;
	t_door	*door;

	l = game->entities;
	while (l)
	{
		door = l->data;
		l = l->next;
		if (door->wall.entity.type != ENTITY_DOOR || door->key != key
			|| !door->key_opens || door->opened || !door->wall.entity.action)
			continue ;
		door->wall.entity.action((t_entity *)door, NULL);
	}
}

static void	update_objective(t_game *game)
{
	t_story	*st;
	int		i;

	st = &game->story;
	i = 0;
	while (i < st->obj_count && st->obj_done[i]
		&& game_has_key(game, st->obj_done[i]))
		i++;
	if (i >= st->obj_count)
		i = st->obj_count - 1;
	if (i != st->obj_current)
	{
		st->obj_current = i;
		st->obj_at = ft_get_time();
		if (i >= 0)
			printf("STORY objective %d: %s\n", i + 1, st->obj_text[i]);
	}
}

void	game_give_key(t_game *game, char key, t_sprite *icon, const char *name)
{
	t_keyring	*kr;
	char		*text;
	char		*msg;

	if (!game || !key || game_has_key(game, key))
		return ;
	kr = &game->keyring;
	if (kr->count >= GAME_MAX_KEYS)
		return ;
	kr->icons[kr->count] = icon;
	kr->keys[kr->count++] = key;
	kr->keys[kr->count] = '\0';
	printf("STORY key %c\n", key);
	text = story_text(name);
	if (text)
	{
		msg = ft_strf("Apanhou: %s", text);
		game_toast(game, msg);
		free(msg);
	}
	free(text);
	open_gates(game, key);
	update_objective(game);
}

/* ------------------------------------------------------------- desenho */

static void	draw_strip(t_ftm_image *c, t_coords o, int h, char *text)
{
	int	w;
	int	pad;

	pad = h / 4;
	w = story_text_w(cub3d()->game->hud.action_font, text, h);
	ftm_draw_rectangle(c, (t_coords){o.x - pad, o.y - pad, 0},
		(t_size){w + pad * 2, h + pad * 2}, (t_ftm_rectangle){0xC0101418,
		0xFFE8C860, (t_size){2, 2}});
}

static void	render_objective(t_game *game, t_ftm_image *c)
{
	t_story	*st;
	char	*line;
	int		h;
	int		pad;
	double	y;

	st = &game->story;
	if (st->obj_current < 0 || st->obj_current >= st->obj_count)
		return ;
	h = story_snap_h(c->size.height * 0.036);
	pad = h / 4;
	y = c->size.height * 0.03 + pad;
	if (st->radio_current >= 0)
		y = c->size.height * (0.03 + 0.15 + 0.02) + pad;
	line = ft_strf("> %s", st->obj_text[st->obj_current]);
	if (!line)
		return ;
	draw_strip(c, (t_coords){c->size.width * 0.03 + pad, y, 0}, h, line);
	story_text_at(c, game->hud.action_font, (t_ftm_text_config){line,
		{c->size.width * 0.03 + pad, y, 0}, h, 0,
		(ft_get_time() - st->obj_at < 2500 && (ft_get_time() / 250) % 2)
		? 0xFFFFFFFF : 0xFFE8C860}, false);
	free(line);
}

static void	render_toast(t_game *game, t_ftm_image *c)
{
	t_keyring	*kr;
	int			h;
	int			w;

	kr = &game->keyring;
	if (!kr->toast[0] || ft_get_time() - kr->toast_at > 2600)
		return ;
	h = story_snap_h(c->size.height * 0.036);
	w = story_text_w(game->hud.action_font, kr->toast, h);
	draw_strip(c, (t_coords){c->size.width / 2 - w / 2,
		c->size.height * 0.40, 0}, h, kr->toast);
	story_text_at(c, game->hud.action_font, (t_ftm_text_config){kr->toast,
		{c->size.width / 2, c->size.height * 0.40, 0}, h, 0, 0xFFF2EEDC},
		true);
}

void	story_render_puzzle(t_game *game, t_ftm_image *canvas)
{
	if (!game || !canvas || game->story.phase != STORY_PLAY
		|| !game->hud.enabled)
		return ;
	if (game->story.obj_count && game->story.obj_current < 0)
		update_objective(game);
	render_objective(game, canvas);
	render_toast(game, canvas);
}

/*
** Chaves na barra de estado: duas casas na caixa entre as balas e a arma
** (x 484-524 e y 6-74 na barra de 640x80, ver scripts/oms_art/hud.py).
*/
void	render_keys(t_game *game, t_ftm_image *canvas, t_size stats_size)
{
	t_ftm_image	*img;
	double		u;
	int			i;
	int			n;
	t_size		s;

	u = stats_size.width / 640.0;
	n = 0;
	i = -1;
	while (++i < game->keyring.count && n < 2)
	{
		if (!game->keyring.icons[i])
			continue ;
		img = get_sprite_image(game->keyring.icons[i]);
		if (!img)
			continue ;
		s = (t_size){28 * u, 28 * u};
		if (img->size.width > img->size.height)
			s.height = s.width * img->size.height / img->size.width;
		else if (img->size.height > img->size.width)
			s.width = s.height * img->size.width / img->size.height;
		ftm_put_image_to_canvas(canvas, img, (t_ftm_pitc_config){
			.coords = {504 * u - s.width / 2.0, canvas->size.height
			- stats_size.height + (24 + n * 32) * u - s.height / 2.0, 0},
			.crop = false, .resize = true, .size = s, .pixel_modifier = NULL});
		n++;
	}
}
