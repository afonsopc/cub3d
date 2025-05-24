/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils16.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:45:53 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/24 14:07:25 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_utils.h>

double	ft_radians(double angle_degree)
{
	return (angle_degree * FT_UTILS_PI / 180.0);
}

double	ft_degrees(double angle_radians)
{
	return (angle_radians * 180.0 / FT_UTILS_PI);
}

double	ft_cos_degrees(double angle_degree)
{
	return (cos(ft_radians(angle_degree)));
}

double	ft_sin_degrees(double angle_degree)
{
	return (sin(ft_radians(angle_degree)));
}

double	ft_tan_degrees(double angle_degree)
{
	return (tan(ft_radians(angle_degree)));
}

