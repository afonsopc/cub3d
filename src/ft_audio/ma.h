/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 15:07:28 by afpachec          #+#    #+#             */
/*   Updated: 2025/07/03 21:51:03 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MA_H
# define MA_H

# define MINIAUDIO_IMPLEMENTATION
# ifdef __EMSCRIPTEN__
#  define MA_EMSCRIPTEN
#  define MA_ENABLE_WEBAUDIO
# endif
# define MA_ENABLE_MP3

# include <miniaudio.h>

#endif
