/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:35:55 by afonsocouti       #+#    #+#             */
/*   Updated: 2025/03/29 14:10:59 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

char	*ft_strndup(const char *s1, size_t n)
{
	char	*dup;
	size_t	i;

	if (!s1)
		return (NULL);
	dup = malloc(n + 1);
	if (!dup)
		return (dup);
	i = -1;
	while (s1[++i] && i < n)
		dup[i] = s1[i];
	dup[i] = '\0';
	return (dup);
}

char	*ft_strdup(const char *s1)
{
	return (ft_strndup(s1, ft_strlen(s1)));
}

bool	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;

	if (!haystack)
		return (NULL);
	if (!needle)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	if (!needle_len)
		return ((char *)haystack);
	if (!len)
		return (NULL);
	while (*haystack && len >= needle_len)
	{
		if (ft_strncmp(haystack, needle, needle_len) == 0)
			return ((char *)haystack);
		haystack++;
		len--;
	}
	return (NULL);
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	return (ft_strnstr(haystack, needle, ft_strlen(haystack)));
}
