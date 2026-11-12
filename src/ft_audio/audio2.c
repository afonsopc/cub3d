/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 13:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 13:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "audio.h"

/*
** Playlist: "a.mp3:b.mp3" played one after the other, then from the start
** again (when looping). The tracks' data sources are chained (miniaudio
** reads the next one in the same callback where the current one ends), so the
** hand-over is sample-exact: no gap, no overlap. Only the first track's
** sound is ever started; fta_update() reports which track is sounding.
*/

static ma_data_source	*source(t_fta_audio *audio, size_t i)
{
	return (ma_sound_get_data_source(&audio->sounds[i]->sound[0]));
}

static void	chain(t_fta_audio *audio)
{
	size_t	i;

	i = -1;
	while (++i + 1 < audio->sounds_count)
		ma_data_source_set_next(source(audio, i), source(audio, i + 1));
	if (audio->config.loop)
		ma_data_source_set_next(source(audio, i), source(audio, 0));
	else
		ma_data_source_set_next(source(audio, i), NULL);
}

t_fta_audio	*fta_playlist_new(const char *paths)
{
	t_fta_audio	*audio;

	audio = fta_audio_new_multi(paths);
	if (!audio || !audio->sounds_count)
		return (audio);
	audio->playlist = true;
	fta_audio_config(audio, audio->config);
	return (audio);
}

void	fta_playlist_play(t_fta_audio *audio)
{
	if (!audio || !audio->sounds_count || !fta_engine()->initialized)
		return ;
	chain(audio);
	audio->running = true;
	ma_sound_start(&audio->sounds[0]->sound[0]);
}

/* call once per frame; true when the playlist moved on to another track */
bool	fta_update(t_fta_audio *audio)
{
	ma_data_source	*cur;
	size_t			i;

	if (!audio || !audio->playlist || !audio->running
		|| !fta_engine()->initialized)
		return (false);
	cur = ma_data_source_get_current(source(audio, 0));
	i = -1;
	while (++i < audio->sounds_count)
		if (source(audio, i) == cur && i != audio->current)
			return (audio->current = i, true);
	return (false);
}
