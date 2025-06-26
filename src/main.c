/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvcordeiro <pvcordeiro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:15:19 by afpachec          #+#    #+#             */
/*   Updated: 2025/06/26 13:30:30 by pvcordeiro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (ft_fputstr(2, "Error\nUsage: ./cub3d <map_file.cub>\n"));
	fte_storage()->exit = cub3d_exit;
	ftm_create_window_e(&cub3d()->window, (t_size){W_WIDTH, W_HEIGHT}, W_TITLE);
	fte_assert();
	cub3d()->curr_map = parse_map_e(argv[1]);
	fte_assert();
	game_load_map_e(&cub3d()->game, &cub3d()->window, cub3d()->curr_map);
	fte_assert();
	ft_backtrack_e(&cub3d()->game);
	fte_assert();
	cub3d()->window.loop_hook = loop;
	cub3d()->window.key_hook = key_hook;
	cub3d()->window.exit_hook = cub3d_exit;
	ftm_update_hooks(&cub3d()->window);
	mlx_loop(cub3d()->window.display);
	cub3d_exit(0);
}
