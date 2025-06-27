/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afpachec <afpachec@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:48:06 by afpachec          #+#    #+#             */
/*   Updated: 2025/05/28 00:22:16 by afpachec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "image.h"

void	ftm_free_image(void *image)
{
	if (!image)
		return ;
	mlx_destroy_image(((t_ftm_image *)image)->display,
		((t_ftm_image *)image)->img_ptr);
	free(image);
}

static bool	load_image_addresses(t_ftm_image *image)
{
	image->data = mlx_get_data_addr(image->img_ptr, &image->bits_per_pixel,
			&image->size_line, &image->endian);
	return (!!image->data);
}

t_ftm_image	*ftm_image_from_file(t_ftm_window *window, char *path)
{
	t_ftm_image	*image;

	image = ft_calloc(1, sizeof(t_ftm_image));
	if (!image)
		return (NULL);
	image->display = window->display;
	image->path = path;
	image->img_ptr = mlx_xpm_file_to_image(image->display, path,
			&image->size.width, &image->size.height);
	if (!image->img_ptr)
		return (free(image), NULL);
	if (!load_image_addresses(image))
		return (ftm_free_image(image), NULL);
	return (image);
}

t_ftm_image	*ftm_image_new(t_ftm_window *window, t_size size)
{
	t_ftm_image	*image;

	image = ft_calloc(1, sizeof(t_ftm_image));
	if (!image)
		return (NULL);
	image->display = window->display;
	image->img_ptr = mlx_new_image(image->display, size.width, size.height);
	if (!image->img_ptr)
		return (free(image), NULL);
	image->size = size;
	if (!load_image_addresses(image))
		return (ftm_free_image(image), NULL);
	return (image);
}

t_list	*ftm_images_from_files(t_ftm_window *window, char **file_paths)
{
	size_t		i;
	t_list		*list;
	t_ftm_image	*tmp_image;

	list = NULL;
	i = -1;
	while (file_paths[++i])
	{
		tmp_image = ftm_image_from_file(window, file_paths[i]);
		if (!tmp_image)
			return (ft_list_destroy(&list), NULL);
		ft_list_add(&list, tmp_image, (void (*)(void *))ftm_free_image);
	}
	return (list);
}
