/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils14.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 15:31:27 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/10 21:58:56 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

bool	ft_list_any(t_list *list, bool (*condition)(void *data, void *params),
		void *params)
{
	t_list	*curr;

	curr = list;
	while (curr && !condition(curr->data, params))
		curr = curr->next;
	return (!!curr);
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

	if (c == 's')
		return (ft_strdup(va_arg(*args, char *)));
	else if (c == 'd')
		return (ft_itoa(va_arg(*args, int)));
	else if (c == 'c')
	{
		c2 = va_arg(*args, int);
		return (ft_strndup(&c2, 1));
	}
	return (NULL);
}

char	*ft_strf(char *format, ...)
{
	va_list	args;
	char	*str;
	char	*to_join;
	char	*tmp;
	size_t	i;

	va_start(args, format);
	str = ft_strdup("");
	if (!str)
		return (NULL);
	i = -1;
	while (format[++i])
	{
		if (format[i] == '%' && format[i + 1])
			to_join = parse_thing(format[++i], &args);
		else
			to_join = ft_strndup(&format[i], 1);
		if (!to_join)
			return (free(str), NULL);
		tmp = ft_strjoin(str, to_join);
		(free(str), free(to_join));
		str = tmp;
	}
	va_end(args);
	return (str);
}
