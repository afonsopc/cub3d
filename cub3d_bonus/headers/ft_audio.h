/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_audio.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 11:01:45 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:05:41 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_AUDIO_H
# define FT_AUDIO_H

// Internal Libs
# include <ft_utils.h>
# include <ft_error.h>

// External Libs
# include <miniaudio.h>

# define FT_AUDIO_SOUND_INSTANCES 50

typedef struct s_fta_audio_config
{
	float	volume;
	bool	loop;
}	t_fta_audio_config;

typedef struct s_fta_audio
{
	ma_sound			sound[FT_AUDIO_SOUND_INSTANCES];
	t_fta_audio_config	config;
	float				length;
}	t_fta_audio;

typedef struct s_fta_engine
{
	bool		initialized;
	ma_engine	engine;
}	t_fta_engine;

t_fta_engine	*fta_engine(void);
void			fta_init_e(void);
void			fta_destroy(void);
t_fta_audio		*fta_audio_new(const char *path);
void			fta_audio_config(t_fta_audio *audio, t_fta_audio_config config);
void			fta_play(t_fta_audio *audio);
void			fta_clear_audio(void *audio);
void			fta_free_audio(void *audio);

#endif
