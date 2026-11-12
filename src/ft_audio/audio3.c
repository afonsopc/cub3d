/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/12 13:00:00 by afpachec          #+#    #+#             */
/*   Updated: 2026/11/12 13:00:00 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "audio.h"
#include <stdio.h>
#include <string.h>

/*
** Gapless MP3: the decoder gives back the LAME "Info" frame as silence, then
** the encoder delay plus 529 samples of decoder delay before the real audio,
** and the padding (minus those 529) after it. The LAME tag stores the delay
** and the padding, so every sound is cut to the exact samples that were
** encoded: a music loop then wraps (or hands over to the next track) with
** no silence in between. The engine decodes at its own rate, so the cut is
** scaled from the file's rate to the rate of the decoded data.
*/

#define MP3_DECODER_DELAY 529

static long	find(const unsigned char *b, long n, const char *tag, long from)
{
	long	i;

	i = from - 1;
	while (++i + 4 <= n)
		if (!memcmp(b + i, tag, 4))
			return (i);
	return (-1);
}

static const int	g_mp3_rates[4][3] = {{11025, 12000, 8000}, {0, 0, 0},
{22050, 24000, 16000}, {44100, 48000, 32000}};

/* skip = samples before the audio, tail = samples after it, at file rate */
static bool	lame_gap(const char *path, ma_uint64 *skip, ma_uint64 *tail,
		int *rate)
{
	unsigned char	b[2048];
	long			n;
	long			s;
	long			x;
	FILE			*f;

	f = fopen(path, "rb");
	if (!f)
		return (false);
	n = (long)fread(b, 1, sizeof(b), f);
	fclose(f);
	s = 0;
	if (n > 10 && !memcmp(b, "ID3", 3))
		s = 10 + ((b[6] & 127) << 21 | (b[7] & 127) << 14
				| (b[8] & 127) << 7 | (b[9] & 127));
	while (s + 4 < n && !(b[s] == 0xFF && (b[s + 1] & 0xE0) == 0xE0))
		s++;
	if (s + 64 > n)
		return (false);
	x = find(b, s + 64, "Info", s);
	if (x < 0)
		x = find(b, s + 64, "Xing", s);
	if (x >= 0)
		x = find(b, n, "LAME", x);
	if (x < 0 || x + 24 > n)
		return (false);
	x += 21;
	*rate = 0;
	if ((b[s + 2] >> 2 & 3) < 3)
		*rate = g_mp3_rates[b[s + 1] >> 3 & 3][b[s + 2] >> 2 & 3];
	*skip = (b[x] << 4 | b[x + 1] >> 4) + MP3_DECODER_DELAY + 576
		* (1 + ((b[s + 1] >> 3 & 3) == 3));
	*tail = (ma_uint64)ft_max(0, ((b[x + 1] & 15) << 8 | b[x + 2])
			- MP3_DECODER_DELAY);
	return (*rate > 0);
}

void	fta_trim_gapless(t_fta_audio *audio, const char *path)
{
	ma_uint64	cut[2];
	ma_uint64	len;
	ma_uint32	out;
	int			rate;
	int			i;

	if (!audio || !path || !ft_str_endswith(path, ".mp3")
		|| !lame_gap(path, &cut[0], &cut[1], &rate)
		|| ma_sound_get_data_format(&audio->sound[0], NULL, NULL, &out, NULL,
			0) != MA_SUCCESS || !out
		|| ma_sound_get_length_in_pcm_frames(&audio->sound[0], &len)
		!= MA_SUCCESS)
		return ;
	cut[0] = (cut[0] * out + rate / 2) / rate;
	cut[1] = (cut[1] * out + rate / 2) / rate;
	if (len <= cut[0] + cut[1] + out / 10)
		return ;
	i = -1;
	while (++i < FT_AUDIO_SOUND_INSTANCES)
		ma_data_source_set_range_in_pcm_frames(ma_sound_get_data_source(
				&audio->sound[i]), cut[0], len - cut[1]);
	audio->length = (float)(len - cut[0] - cut[1]) * 1000 / out;
}
