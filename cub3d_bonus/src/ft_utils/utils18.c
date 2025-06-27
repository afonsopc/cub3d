/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils18.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:40:59 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:10:55 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

bool	ft_str_equal_char_ptr(const char *str, const char *char_ptr)
{
	bool	result;
	char	c_str[2];

	if (str == char_ptr)
		return (true);
	if (!str || !char_ptr)
		return (false);
	c_str[0] = *char_ptr;
	c_str[1] = '\0';
	result = ft_strequal(str, c_str);
	return (result);
}

char	*ft_strf(const char *format, ...)
{
	va_list	args;
	char	*str;

	va_start(args, format);
	str = ft_strfva(format, &args);
	va_end(args);
	return (str);
}

char	*ft_strnrstr(const char *haystack, const char *needle, size_t len)
{
	size_t	h_len;
	size_t	n_len;
	size_t	i;

	if (!haystack || !needle || !len)
		return (NULL);
	n_len = ft_strlen(needle);
	if (!n_len)
		return ((char *)haystack);
	h_len = ft_strlen(haystack);
	if (len > h_len)
		len = h_len;
	i = len - n_len;
	while (true)
	{
		if (!ft_strncmp(&haystack[i], needle, n_len))
			return ((char *)&haystack[i]);
		if (!i)
			break ;
		i--;
	}
	return (NULL);
}

char	*ft_strrstr(const char *haystack, const char *needle)
{
	return (ft_strnrstr(haystack, needle, ft_strlen(haystack)));
}

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;
	char	*last_occurrence;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	last_occurrence = NULL;
	while (len--)
		if (s[len] == (char)c)
			last_occurrence = (char *)&s[len];
	return (last_occurrence);
}
