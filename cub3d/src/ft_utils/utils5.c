/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:49:52 by pvcordeiro        #+#    #+#             */
/*   Updated: 2025/06/22 11:50:14 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

static void	free_all(char **strs)
{
	size_t	i;

	i = -1;
	while (strs[++i])
		free(strs[i]);
}

static int	count_items(const char *s1, const char *s2)
{
	size_t	count;

	count = 0;
	while (*s1)
	{
		if (!ft_strchr(s2, *s1) && (ft_strchr(s2, *(s1 + 1)) || !*(s1 + 1)))
			count++;
		s1++;
	}
	return (count);
}

static int	split(const char *s1, const char *s2, char **strs)
{
	size_t		i;
	const char	*start;

	i = 0;
	while (*s1 && ft_strchr(s2, *s1))
		s1++;
	while (*s1)
	{
		start = s1;
		while (*s1 && !ft_strchr(s2, *s1))
			s1++;
		strs[i] = malloc(s1 - start + 1);
		if (!strs[i])
			return (free_all(strs), 0);
		ft_strlcpy(strs[i], start, s1 - start + 1);
		while (*s1 && ft_strchr(s2, *s1))
			s1++;
		i++;
	}
	strs[i] = NULL;
	return (1);
}

char	**ft_split(const char *s1, const char *s2)
{
	char	**strs;

	if (!s1 || !s2)
		return (NULL);
	strs = ft_calloc(count_items(s1, s2) + 1, sizeof(char *));
	if (!strs)
		return (NULL);
	if (!split(s1, s2, strs))
		return (free(strs), NULL);
	return (strs);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}
