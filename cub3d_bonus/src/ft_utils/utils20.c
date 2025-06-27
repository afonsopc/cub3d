/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils20.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 01:05:13 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/20 02:20:16 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

double	ft_fraction(double value)
{
	if (value < 0)
		return (1.0 + value - floor(value));
	return (value - floor(value));
}

void	*ft_ternary_ptr(void *con, void *_if, void *_else)
{
	if (con)
		return (_if);
	return (_else);
}

double	ft_ternary_double(bool con, double _if, double _else)
{
	if (con)
		return (_if);
	return (_else);
}

int	ft_ternary_int(bool con, int _if, int _else)
{
	if (con)
		return (_if);
	return (_else);
}

void	ft_list_foreach(t_list *list, void (*f)(void *, void *param),
	void *param)
{
	t_list	*current;

	if (!list || !f)
		return ;
	current = list;
	while (current)
	{
		f(current->data, param);
		current = current->next;
	}
}
