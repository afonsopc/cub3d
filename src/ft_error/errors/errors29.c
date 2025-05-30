/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors29.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:25:55 by afpachec          #+#    #+#             */
/*   Updated: 2025/04/26 22:26:16 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_error.h>

char	*get_error_message30(t_error _error);

char	*get_error_message29(t_error _error)
{
	if (_error == ERROR_INVALID_AUDIO_FORMAT)
		return (ERROR_INVALID_AUDIO_FORMAT_MSG);
	return (get_error_message30(_error));
}
