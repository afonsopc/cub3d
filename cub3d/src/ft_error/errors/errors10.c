/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors10.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:25:55 by afpachec          #+#    #+#             */
/*   Updated: 2025/04/26 22:26:16 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_error.h>

char	*get_error_message11(t_error _error);

char	*get_error_message10(t_error _error)
{
	if (_error == ERROR_MAP_HASHMAP)
		return (ERROR_MAP_HASHMAP_MSG);
	return (get_error_message11(_error));
}
