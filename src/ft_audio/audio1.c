/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:58:59 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:59:59 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "audio.h"

void	fta_audio_config(t_fta_audio *audio, t_fta_audio_config config)
{
	int		i;
	size_t	j;

	if (!audio || !fta_engine()->initialized)
		return ;
	if (config.volume < 0.0)
		config.volume = 0.0;
	else if (config.volume > 1.0)
		config.volume = 1.0;
	audio->config = config;
	if (audio->sounds_count > 0)
	{
		j = 0;
		while (j < audio->sounds_count)
			fta_audio_config(audio->sounds[j++], config);
		return ;
	}
	i = -1;
	while (++i < FT_AUDIO_SOUND_INSTANCES)
	{
		ma_sound_set_looping(&audio->sound[i], config.loop);
		ma_sound_set_volume(&audio->sound[i], config.volume);
	}
}

void	fta_clear_audio(void *audio)
{
	int				i;
	size_t			j;
	t_fta_audio		*aud;

	if (!audio || !fta_engine()->initialized)
		return ;
	aud = (t_fta_audio *)audio;
	if (aud->sounds_count > 0)
	{
		j = 0;
		while (j < aud->sounds_count)
		{
			if (aud->sounds[j])
				fta_free_audio(aud->sounds[j]);
			j++;
		}
		free(aud->sounds);
		return ;
	}
	i = -1;
	while (++i < FT_AUDIO_SOUND_INSTANCES)
		ma_sound_uninit(&aud->sound[i]);
}

void	fta_free_audio(void *audio)
{
	fta_clear_audio(audio);
	free(audio);
}

void	fta_play(t_fta_audio *audio)
{
	static int	i;
	t_fta_audio	*selected_audio;
	int			random_index;

	if (!audio || !fta_engine()->initialized)
		return ;
	if (audio->sounds_count > 0)
	{
		random_index = rand() % audio->sounds_count;
		selected_audio = audio->sounds[random_index];
		fta_play(selected_audio);
		return ;
	}
	++i;
	if (i < 0 || i >= FT_AUDIO_SOUND_INSTANCES)
		i = 0;
	ma_sound_start(&audio->sound[i]);
}
