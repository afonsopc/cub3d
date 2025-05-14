/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors25.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:25:55 by afpachec          #+#    #+#             */
/*   Updated: 2025/04/26 22:26:16 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_error.h>

char	*get_error_message26(t_error _error);

char	*get_error_message25(t_error _error)
{
	if (_error == ERROR_INIT_DOOR_FRAME)
		return (ERROR_INIT_DOOR_FRAME_MSG);
	return (get_error_message26(_error));
}
