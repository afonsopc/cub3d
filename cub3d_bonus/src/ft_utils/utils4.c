/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 00:32:15 by afonsocouti       #+#    #+#             */
/*   Updated: 2025/04/27 22:38:52 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

bool	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size != 0 && count > (size_t)-1 / size)
		return (NULL);
	ptr = malloc(count * size);
	if (ptr)
		ft_bzero(ptr, count * size);
	return (ptr);
}

size_t	ft_strvlen(char **v)
{
	size_t	i;

	if (!v)
		return (0);
	i = 0;
	while (v[i])
		i++;
	return (i);
}

char	**ft_strvjoin(char **v1, char **v2)
{
	char	**new;
	ssize_t	i;
	ssize_t	j;

	if (!v1 && !v2)
		return (NULL);
	if (!v1)
		return (ft_strvdup(v2));
	if (!v2)
		return (ft_strvdup(v1));
	i = ft_strvlen(v1);
	j = ft_strvlen(v2);
	new = ft_calloc(i + j + 1, sizeof(char *));
	if (!new)
		return (NULL);
	i = -1;
	while (v1[++i])
		new[i] = ft_strdup(v1[i]);
	j = -1;
	while (v2[++j])
		new[i + j] = ft_strdup(v2[j]);
	return (new);
}
