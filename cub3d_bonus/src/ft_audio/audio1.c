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
	int	i;

	if (!audio || !fta_engine()->initialized)
		return ;
	if (config.volume < 0.0)
		config.volume = 0.0;
	else if (config.volume > 1.0)
		config.volume = 1.0;
	audio->config = config;
	i = -1;
	while (++i < FT_AUDIO_SOUND_INSTANCES)
	{
		ma_sound_set_looping(&audio->sound[i], config.loop);
		ma_sound_set_volume(&audio->sound[i], config.volume);
	}
}

void	fta_clear_audio(void *audio)
{
	int	i;

	if (!audio || !fta_engine()->initialized)
		return ;
	i = -1;
	while (++i < FT_AUDIO_SOUND_INSTANCES)
		ma_sound_uninit(&((t_fta_audio *)audio)->sound[i]);
}

void	fta_free_audio(void *audio)
{
	fta_clear_audio(audio);
	free(audio);
}

void	fta_play(t_fta_audio *audio)
{
	static int	i;

	if (!audio || !fta_engine()->initialized)
		return ;
	++i;
	if (i < 0 || i >= FT_AUDIO_SOUND_INSTANCES)
		i = 0;
	ma_sound_start(&audio->sound[i]);
}
