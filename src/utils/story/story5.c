/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   story5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 13:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 13:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "story.h"

/*
** Music the map does not set itself (the .cub keys always win):
**   STATS: the end-of-level stats screen of every story map;
**   BACKGROUND: a playlist per map, its songs one after the other, gapless.
**   "*" is the map's own BACKGROUND_SOUND; a map without one gets the list.
** Files are the chip loops of scripts/oms_art/music (see loops.json).
*/
#define MUSIC_DIR "assets/oms3d/sounds/music/"
#define STATS_MUSIC "a-nossa-terra"

static const char	*g_playlists[][2] = {
{"hub.cub", "hora-da-despedida:despedida:fado-angola"},
{"missao1.cub", "*:ressurreicao:hino-mocidade"
	":hino-fuzileiros:soldados-de-portugal"},
{"missao1_42.cub", "*:ressurreicao:hino-mocidade"
	":hino-fuzileiros:soldados-de-portugal"},
{"missao2.cub", "*:patria-ditosa:mocambique-e-portugal:adeus-guine"},
{"missao1_mega.cub", "*:ressurreicao"},
{"sbj.cub", "*:hino-fuzileiros"},
{NULL, NULL}
};

static t_fta_audio	*load(t_game *game, char *key, char *paths, float vol)
{
	t_fta_audio	*audio;

	if (ft_strchr(paths, ':'))
		audio = fta_playlist_new(paths);
	else
		audio = fta_audio_new(paths);
	if (!audio)
		return (NULL);
	fta_audio_config(audio, (t_fta_audio_config){vol, true});
	ft_hashmap_set(game->sounds, key, audio, fta_free_audio);
	return (audio);
}

/* "*:hino-legiao" -> "<map's own>:assets/.../hino-legiao.mp3" */
static char	*expand(const char *list, char *own)
{
	char	**names;
	char	*out;
	char	*tmp;
	char	*item;
	int		i;

	names = ft_split(list, ":");
	if (!names)
		return (NULL);
	out = ft_strdup("");
	i = -1;
	while (out && names[++i])
	{
		if (ft_strequal(names[i], "*") && !own)
			continue ;
		if (ft_strequal(names[i], "*"))
			item = ft_strdup(own);
		else
			item = ft_strf(MUSIC_DIR "%s.mp3", names[i]);
		tmp = out;
		out = ft_strf("%s%s%s", tmp, *tmp ? ":" : "", item);
		(free(tmp), free(item));
	}
	return (ft_strvfree(names), out);
}

static const char	*playlist_for(t_game *game)
{
	char	*path;
	char	*base;
	int		i;

	path = game->map->path;
	if (!path)
		return (NULL);
	base = ft_strrchr(path, '/');
	if (base)
		base++;
	else
		base = path;
	i = -1;
	while (g_playlists[++i][0])
		if (ft_strequal(g_playlists[i][0], base))
			return (g_playlists[i][1]);
	return (NULL);
}

static void	background(t_game *game)
{
	const char	*list;
	char		*own;
	char		*paths;
	float		vol;

	list = playlist_for(game);
	if (!list)
		return ;
	own = ft_hashmap_get_value(game->map->types, "BACKGROUND_SOUND");
	if (own && ft_strchr(own, ':'))
		return ;
	vol = 0.3;
	if (game->background_sound)
		vol = game->background_sound->config.volume;
	paths = expand(list, own);
	if (paths && *paths && load(game, "BACKGROUND", paths, vol))
		game->background_sound = ft_hashmap_get_value(game->sounds,
				"BACKGROUND");
	free(paths);
}

void	story_music_init(t_game *game)
{
	if (!fta_engine()->initialized)
		return ;
	background(game);
	if (game->story.level_name && !ft_hashmap_get_value(game->sounds, "STATS"))
		load(game, "STATS", MUSIC_DIR STATS_MUSIC ".mp3", 0.4);
}
