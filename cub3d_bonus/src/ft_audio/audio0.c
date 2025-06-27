/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 11:02:20 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:01:58 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "audio.h"

t_fta_engine	*fta_engine(void)
{
	static t_fta_engine	engine;

	return (&engine);
}

void	fta_init_e(void)
{
	ma_result	result;

	fte_set(NULL);
	result = ma_engine_init(NULL, &fta_engine()->engine);
	if (result != MA_SUCCESS)
		return (fte_set("audio engine init"));
	fta_engine()->initialized = true;
}

void	fta_destroy(void)
{
	if (!fta_engine()->initialized)
		return ;
	ma_engine_uninit(&fta_engine()->engine);
	fta_engine()->initialized = false;
}

static bool	init_audio_instances(t_fta_audio *audio)
{
	ma_result	result;
	int			i;

	i = 0;
	while (++i < FT_AUDIO_SOUND_INSTANCES)
		result = ma_sound_init_copy(&fta_engine()->engine, &audio->sound[0],
				0, NULL, &audio->sound[i]);
	return (result == MA_SUCCESS);
}

t_fta_audio	*fta_audio_new(const char *path)
{
	t_fta_audio	*audio;
	ma_result	result;

	if (!fta_engine()->initialized)
		return (NULL);
	audio = ft_calloc(1, sizeof(t_fta_audio));
	if (!audio)
		return (NULL);
	audio->config.volume = 1.0;
	result = ma_sound_init_from_file(&fta_engine()->engine,
			path, 0, NULL, NULL, &audio->sound[0]);
	if (result != MA_SUCCESS)
		return (free(audio), NULL);
	if (!init_audio_instances(audio))
		return (fta_free_audio(audio), NULL);
	result = ma_sound_get_length_in_seconds(&audio->sound[0], &audio->length);
	if (result != MA_SUCCESS)
		return (free(audio), NULL);
	audio->length *= 1000;
	return (audio);
}
