/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 15:18:01 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/27 12:11:46 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

void	ft_close(int fd)
{
	if (fd > 2)
		close(fd);
}

void	ft_close2(int fd1, int fd2)
{
	ft_close(fd1);
	ft_close(fd2);
}

void	ft_fprint_strv(int fd, char **strv)
{
	int		i;
	bool	multi_line;

	i = 0;
	multi_line = ft_strvlen(strv) > 5;
	ft_fputstr(fd, "[");
	while (strv && strv[i])
	{
		if (multi_line)
			ft_fputstr(fd, "\n\t");
		ft_fputstr(fd, "\"");
		ft_fputstr(fd, strv[i]);
		ft_fputstr(fd, "\"");
		if (strv[i + 1])
			ft_fputstr(fd, ", ");
		else if (multi_line)
			ft_fputstr(fd, "\n");
		i++;
	}
	ft_fputstr(fd, "]\n");
}

bool	ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}

char	*ft_strappend(char *str, char append)
{
	char	*tmp;
	char	*appended;

	tmp = ft_strndup(&append, 1);
	appended = ft_strjoin(str, tmp);
	free(tmp);
	return (appended);
}
