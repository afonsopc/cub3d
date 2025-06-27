/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 00:46:10 by afonsocouti       #+#    #+#             */
/*   Updated: 2025/05/08 14:57:52 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

char	**ft_strvndup(char **v, size_t n)
{
	char	**new;
	size_t	i;

	if (!v || !n)
		return (NULL);
	new = ft_calloc(n + 1, sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (i < n && v[i])
	{
		new[i] = ft_strdup(v[i]);
		++i;
	}
	return (new);
}

char	**ft_strvdup(char **v)
{
	return (ft_strvndup(v, ft_strvlen(v)));
}

void	ft_strvfree(char **v)
{
	int	i;

	if (!v)
		return ;
	i = -1;
	while (v[++i])
		free(v[i]);
	free(v);
	return ;
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	if (!s)
		return ;
	ptr = s;
	while (n > 0)
	{
		ptr[n - 1] = 0;
		n--;
	}
}

char	*ft_charjoin(char *str, char c)
{
	char	*result;
	int		len;

	len = ft_strlen(str);
	result = ft_calloc(sizeof(char), (len + 2));
	if (!result)
		return (NULL);
	ft_memcpy(result, str, len);
	result[len] = c;
	result[len + 1] = '\0';
	return (result);
}
