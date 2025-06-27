/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:00:34 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 14:58:39 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_error.h>

t_error_storage	*fte_storage(void)
{
	static t_error_storage	singleton;

	if (!singleton.exit)
		singleton.exit = exit;
	return (&singleton);
}

void	fte_set(const char *msg, ...)
{
	char	*tmp;
	va_list	args;

	ft_bzero(fte_storage()->msg, 1024);
	va_start(args, msg);
	tmp = ft_strfva(msg, &args);
	va_end(args);
	ft_strlcpy(fte_storage()->msg, tmp, 1024);
	free(tmp);
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
	return (fte_storage()->msg[0]);
}

void	fte_assert(void)
{
	if (!fte_flagged())
		return ;
	ft_error_fputstr(STDERR_FILENO, "Error\n");
	ft_error_fputstr(STDERR_FILENO, fte_storage()->msg);
	ft_error_fputstr(STDERR_FILENO, "\n");
	fte_storage()->exit(1);
}
