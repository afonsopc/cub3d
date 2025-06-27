/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors21.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:25:55 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 14:03:02 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_error.h>

char	*get_error_message22(t_error _error);

char	*get_error_message21(t_error _error)
{
	if (_error == ERROR_INVALID_FILETYPE)
		return (ERROR_INVALID_FILETYPE_MSG);
	return (get_error_message21(_error));
}
