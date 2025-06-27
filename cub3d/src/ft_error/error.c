/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:00:34 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/07 23:21:37 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_error.h>

t_error_storage	*fte_storage(void)
{
	static t_error_storage	singleton;

	if (singleton.exit == NULL)
		singleton.exit = exit;
	return (&singleton);
}

void	fte_set(t_error _error)
{
	fte_storage()->error = _error;
}

static ssize_t	ft_error_fputstr(int fd, char *string)
{
	size_t	len;

	len = 0;
	while (string[len])
		++len;
	return (write(fd, string, len));
}

bool	fte_flagged(void)
{
	if (fte_storage()->error == ERROR_NO_ERROR)
		return (false);
	return (true);
}

void	fte_assert(void)
{
	if (!fte_flagged())
		return ;
	ft_error_fputstr(STDERR_FILENO, "Error\n");
	ft_error_fputstr(STDERR_FILENO,
		fte_error_to_message(fte_storage()->error));
	ft_error_fputstr(STDERR_FILENO, "\n");
	fte_storage()->exit(fte_storage()->error);
}
