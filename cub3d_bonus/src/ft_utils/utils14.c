/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils14.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 15:31:27 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/25 15:11:27 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

bool	ft_list_any(t_list *list,
	bool (*condition)(void *data, void *params), void *params)
{
	while (list)
	{
		if (condition(list->data, params))
			return (true);
		list = list->next;
	}
	return (false);
}

bool	ft_str_all_params(char *str, bool (*func)(char, void *), void *params)
{
	size_t	i;

	if (!str || !func)
		return (false);
	i = -1;
	while (str[++i])
		if (!func(str[i], params))
			return (false);
	return (true);
}

static char	*parse_thing(char c, va_list *args)
{
	char	c2;
	char	*result;

	result = NULL;
	if (c == 's')
	{
		result = ft_strdup(va_arg(*args, char *));
		if (!result)
			result = ft_strdup("(null)");
	}
	else if (c == 'd')
		result = ft_itoa(va_arg(*args, int));
	else if (c == 'c')
	{
		c2 = va_arg(*args, int);
		result = ft_strndup(&c2, 1);
	}
	else if (c == '%')
		result = ft_strdup("%");
	return (result);
}

char	*ft_strfva(const char *format, va_list *args)
{
	char		*str;
	char		*to_join;
	char		*tmp;
	size_t		i;

	str = ft_strdup("");
	if (!format || !str)
		return (free(str), NULL);
	i = -1;
	while (format[++i])
	{
		if (format[i] == '%' && format[i + 1])
			to_join = parse_thing(format[++i], args);
		else
			to_join = ft_strndup(&format[i], 1);
		if (!to_join)
			return (free(str), NULL);
		tmp = ft_strjoin(str, to_join);
		(free(str), free(to_join));
		str = tmp;
	}
	return (str);
}

double	ft_atof(const char *str)
{
	if (!str)
		return (0.0);
	return (atof(str));
}
