/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils13.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:52:26 by paude-so          #+#    #+#             */
/*   Updated: 2025/05/10 21:09:12 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

bool	ft_str_all(char *str, bool (*func)(char))
{
	size_t	i;

	if (!str || !func)
		return (false);
	i = -1;
	while (str[++i])
		if (!func(str[i]))
			return (false);
	return (true);
}

bool	ft_str_endswith(char *str, char *ending)
{
	size_t	str_len;
	size_t	ending_len;

	str_len = ft_strlen(str);
	ending_len = ft_strlen(ending);
	if (str_len < ending_len)
		return (false);
	return (ft_strcmp(str + (str_len - ending_len), ending) == 0);
}

char	*ft_strtrim(char *s)
{
	char	*start;
	char	*end;
	size_t	len;

	if (!s)
		return (NULL);
	start = s;
	end = s + ft_strlen(s) - 1;
	while (*start && ft_isspace(*start))
		start++;
	while (end > start && ft_isspace(*end))
		end--;
	len = end - start + 1;
	return (ft_strndup(start, len));
}

t_time	ft_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_fprint_list(int fd, t_list *list, char *(*to_str)(void *data))
{
	char	**strv;

	if (!list)
		return ;
	strv = ft_list_to_strv(list, to_str);
	if (!strv)
		return ;
	ft_fprint_strv(fd, strv);
	free(strv);
}
